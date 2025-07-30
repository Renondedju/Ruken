#include "JobSystem/Queues/JobQueue.hpp"
#include "JobSystem/Concurrency.hpp"
#include "JobSystem/WorkerInfo.hpp"

#include <array>
#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

USING_RUKEN_NAMESPACE

static inline std::array<const RkChar*, 2> s_request_plots {"Main Requests", "IO Requests"};

JobQueue::JobQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{}

RkBool JobQueue::TryConsumeWorkerRequest(Concurrency& inout_concurrency) noexcept
{
    do
    {
        // Checking if the calling worker is needed to meet the requirements of the queue.
        if (inout_concurrency.ComputeRequest(+1) < 0)
        {
            inout_concurrency.packed_value = m_concurrency.fetch_sub(s_one_requested_concurrency, std::memory_order_acq_rel) - s_one_requested_concurrency;

            TracyPlot(s_request_plots[m_request_location.path], static_cast<int64_t>(inout_concurrency.fields.requested));
            return false;
        }

        // If the caller is needed then we need to update the concurrency of the queue
    } while(!m_concurrency.compare_exchange_weak(inout_concurrency,
        inout_concurrency + s_one_current_concurrency - s_one_requested_concurrency, std::memory_order_acq_rel)
    );

    TracyPlot(s_request_plots[m_request_location.path], static_cast<int64_t>(inout_concurrency.fields.requested));

    return true;
}

RkVoid JobQueue::TryConsumeJob(RkUint32 const in_max_attempts) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    std::coroutine_handle job {};

    // Attempting to pop a job
    for (RkUint32 attempts = 0; attempts <= in_max_attempts && !m_queue.try_pop(job); ++attempts)
        atomic_queue::spin_loop_pause();

    // Escaping timeouts
    if (!job) return;

    // Running the job
    m_concurrency.fetch_sub(s_one_optimal_concurrency.packed_value, std::memory_order_acq_rel);
    job.resume();
}

RkVoid JobQueue::OnRegister(WorkerRequestTree& in_request_tree, BinaryTreePath const& in_request_location) noexcept
{
    m_request_tree     = &in_request_tree;
    m_request_location = in_request_location;
}

RkBool JobQueue::TryEmitWorkerRequest(Concurrency& inout_concurrency) noexcept
{
    do
    {
        // Checking if we need to request a worker while anticipating the fact that
        // the current worker could exit the queue without checking if it's still needed.
        if (inout_concurrency.ComputeRequest(inout_concurrency.fields.requested) <= 0)
            return false;

        // Worker request CAS
    } while (!m_concurrency.compare_exchange_weak(inout_concurrency,
        inout_concurrency + s_one_requested_concurrency, std::memory_order_acq_rel)
    );

    m_request_tree->EmitRequest(m_request_location);
    TracyPlot(s_request_plots[m_request_location.path], static_cast<int64_t>(inout_concurrency.fields.requested));

    return true;
}

RkVoid JobQueue::Push(std::coroutine_handle<> in_handle) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    // Adding a new job to the queue
    m_queue.push(std::forward<std::coroutine_handle<>>(in_handle));

    Concurrency concurrency {
        .packed_value = m_concurrency.fetch_add(s_one_optimal_concurrency, std::memory_order_acq_rel)
            + s_one_optimal_concurrency
    };

    TryEmitWorkerRequest(concurrency);
}

RkVoid JobQueue::RunMultiple(std::stop_token const&)
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    Concurrency concurrency { .packed_value = m_concurrency.load(std::memory_order_acquire) };

    // If we could not consume a request, that means the worker is not
    // needed for this queue anymore, another worker has been faster.
    if (!TryConsumeWorkerRequest(concurrency))
        return;

    //

    // do
    // {
        //// Inner loop consumes jobs and checks if the queue still needs us.
        //while (concurrency.ComputeRequest(-1) > 0 && !in_stop_token.stop_requested())
        //{
            // Consuming a maximum of 10 jobs before checking if we are still needed
            for (int tasks = 0; tasks < 100; tasks++)
                TryConsumeJob(50);

            // Checking if the queue still needs us
            // concurrency.packed_value = m_concurrency.load(std::memory_order_acquire);
        //}

    // The outer loop makes sure only one thread exits the queue at the same time to avoid overshooting requests.
    // } while(!m_concurrency.compare_exchange_weak(concurrency,
    //     concurrency - s_one_current_concurrency, std::memory_order_acq_rel)
    // );

    concurrency.packed_value = m_concurrency.fetch_sub(s_one_current_concurrency, std::memory_order_acq_rel) - s_one_current_concurrency;

    // Since we forcefully exited the queue, there is a possibility that we are
    // leaving the queue with jobs in it but no requests were made.
    // So we need to make sure to emit a request if this is the case.
    TryEmitWorkerRequest(concurrency);
}

RkVoid JobQueue::RunOnce()
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    Concurrency concurrency { .packed_value = m_concurrency.load(std::memory_order_acquire) };

    // If we could not consume a request, that means the worker is not
    // needed for this queue anymore, another worker has been faster.
    if (!TryConsumeWorkerRequest(concurrency))
        return;

    // Running a single job
    TryConsumeJob(50);

    concurrency.packed_value = m_concurrency.fetch_sub(s_one_current_concurrency, std::memory_order_acq_rel) - s_one_current_concurrency;

    // Since we forcefully exited the queue, there is a possibility that we are
    // leaving the queue with jobs in it but no requests were made.
    // So we need to make sure to emit a request if this is the case.
    TryEmitWorkerRequest(concurrency);
}

RkVoid JobQueue::SetMaximumConcurrency(RkUint16 const in_max_concurrency) noexcept
{
    Concurrency concurrency { .packed_value = m_concurrency.load(std::memory_order_acquire) };
    Concurrency desired     {};

    do
    {
        desired.packed_value   = concurrency.packed_value;
        desired.fields.maximum = in_max_concurrency;
    } while (!m_concurrency.compare_exchange_weak(concurrency, desired, std::memory_order_relaxed));
}
