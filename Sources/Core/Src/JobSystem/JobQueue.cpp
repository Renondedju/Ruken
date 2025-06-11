#include "JobSystem/Queues/JobQueue.hpp"
#include "JobSystem/Concurrency.hpp"
#include "JobSystem/WorkerInfo.hpp"

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

USING_RUKEN_NAMESPACE

JobQueue::JobQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{}

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

RkVoid JobQueue::Push(std::coroutine_handle<> in_handle) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    m_queue.push(std::forward<std::coroutine_handle<>>(in_handle));

    // Adding a new job to the queue
    Concurrency concurrency {
        .packed_value = m_concurrency.fetch_add(s_one_optimal_concurrency.packed_value, std::memory_order_acq_rel)
                                              + s_one_optimal_concurrency.packed_value
    };

    do
    {
        // Checking if we need to request a worker
        if (concurrency.ComputeRequest(concurrency.fields.requested) <= 0)
            return;

        // Worker request CAS
    } while (!m_concurrency.compare_exchange_weak(concurrency.packed_value,
        concurrency.packed_value + s_one_requested_concurrency.packed_value, std::memory_order_acq_rel));

    m_request_tree->EmitRequest(m_request_location);
}

RkVoid JobQueue::Pop(RkBool const in_greedy, std::stop_token const& in_stop_token) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    Concurrency concurrency {
        .packed_value = m_concurrency.fetch_sub(s_one_requested_concurrency.packed_value, std::memory_order_acq_rel)
    };

    do
    {
        // Checking if the calling worker is needed to meet the requirements of the queue.
        if (concurrency.ComputeRequest(+1) < 0)
            return;

        // If the caller is needed then we need to update the concurrency of the queue
    } while(!m_concurrency.compare_exchange_weak(concurrency.packed_value, concurrency.packed_value
        + s_one_current_concurrency.packed_value, std::memory_order_acq_rel));

    do
    {
        // Inner loop consumes jobs and checks if the queue still needs us.
        if (!in_greedy)
            TryConsumeJob(50);

        else while (concurrency.ComputeRequest(-1) > 0 && !in_stop_token.stop_requested())
        {
            // Consuming a maximum of 10 jobs before checking if we are still needed
            for (int tasks = 0; tasks < 10; tasks++)
                TryConsumeJob(50);

            // Checking if the queue still needs us
            concurrency.packed_value = m_concurrency.load(std::memory_order_acquire);
        }

    // The outer loop makes sure only one thread exits the queue at the same time to avoid overshooting requests.
    } while(!m_concurrency.compare_exchange_weak(concurrency.packed_value,
        concurrency.packed_value - s_one_current_concurrency.packed_value, std::memory_order_acq_rel));
}

RkVoid JobQueue::SetMaximumConcurrency(RkUint16 const in_max_concurrency) noexcept
{
    Concurrency concurrency { .packed_value = m_concurrency.load(std::memory_order_acquire) };
    Concurrency desired     {};

    do
    {
        desired.packed_value   = concurrency.packed_value;
        desired.fields.maximum = in_max_concurrency;
    } while (!m_concurrency.compare_exchange_weak(concurrency.packed_value, desired.packed_value, std::memory_order_relaxed));
}
