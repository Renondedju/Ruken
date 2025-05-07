#include "ExecutiveSystem/Queues/JobQueue.hpp"
#include "ExecutiveSystem/WorkerInfo.hpp"

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

USING_RUKEN_NAMESPACE

constexpr auto g_request = "Threads requested";
constexpr auto g_current = "Current concurrency";
constexpr auto g_optimal = "Optimal concurrency";

JobQueue::JobQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{
    if constexpr (RUKEN_TRACE_SHOW_WORKER_ZONES)
    {
        TracyPlotConfig(g_request, tracy::PlotFormatType::Number, true, false, 0);
        TracyPlotConfig(g_current, tracy::PlotFormatType::Number, true, false, 0);
        TracyPlotConfig(g_optimal, tracy::PlotFormatType::Number, true, false, 0);
    }
}

RkVoid JobQueue::TryConsumeJob(RkUint32 const in_max_attempts) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    std::coroutine_handle<>      job;
    RkBool                       has_job {false};
    RkSize                       remaining_attempts { static_cast<RkSize>(in_max_attempts) + 1ULL };

    // Attempting to pop a job
    while (--remaining_attempts > 0 && !has_job)
    {
        has_job = m_queue.try_pop(job);
        atomic_queue::spin_loop_pause();
    }

    // Escaping timeouts
    if (remaining_attempts == 0 && !has_job)
        return;

    // Otherwise we need to run the job and update the concurrency
    job.resume();

    ConcurrencyCounter constexpr one_optimal { {.current_concurrency = 0, .optimal_concurrency = 1} };
    ConcurrencyCounter const     counter     { .value = m_concurrency.fetch_sub(one_optimal.value, std::memory_order_acq_rel) };

    if constexpr (RUKEN_TRACE_SHOW_WORKER_ZONES)
    {
        TracyCPlotF(g_request, GetSignedConcurrencyRequest(counter))
        TracyCPlotI(g_current, counter.current_concurrency)
        TracyCPlotI(g_optimal, counter.optimal_concurrency)
    }
}

RkFloat JobQueue::GetSignedConcurrencyRequest(ConcurrencyCounter const& in_concurrency, RkUint32 const in_offset) const noexcept
{
    RkFloat value = ComputeOptimalConcurrency(in_concurrency.optimal_concurrency);
    value        -=      static_cast<RkFloat>(in_concurrency.current_concurrency + in_offset);

    return value;
}

RkVoid JobQueue::Yield(std::stop_token const& in_stop_token) const noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    if (in_stop_token.stop_requested())
        return;

    std::stop_callback stop_callback {
        in_stop_token, [&] {
            m_condition_variable.notify_all();
        }
    };

    std::unique_lock lock(m_sleep_mutex);
    m_condition_variable.wait(lock, [&]() -> RkBool {
        return in_stop_token.stop_requested() || GetSignedConcurrencyRequest(ConcurrencyCounter {
             .value = m_concurrency.load(std::memory_order_acquire)
        }) > 0.0f;
    });
}

RkVoid JobQueue::Push(std::coroutine_handle<> in_handle) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    m_queue.push(std::forward<std::coroutine_handle<>>(in_handle));

    ConcurrencyCounter constexpr one_optimal { {.current_concurrency = 0, .optimal_concurrency = 1} };
    ConcurrencyCounter const     current     { .value = m_concurrency.fetch_add(one_optimal.value, std::memory_order_acq_rel) };

    if constexpr (RUKEN_TRACE_SHOW_WORKER_ZONES)
    {
        TracyCPlotF(g_request, GetSignedConcurrencyRequest(current))
        TracyCPlotI(g_current, current.current_concurrency)
        TracyCPlotI(g_optimal, current.optimal_concurrency)
    }

    if (GetSignedConcurrencyRequest(current) > 0.0f)
        m_condition_variable.notify_one();
}

RkVoid JobQueue::PopAndRun(RkBool const in_greedy, std::stop_token const& in_stop_token) noexcept
{
    ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

    ConcurrencyCounter           counter     { .value = m_concurrency.load(std::memory_order_acquire) };
    ConcurrencyCounter constexpr one_current { {.current_concurrency = 1, .optimal_concurrency = 0} };

    do
    {
        // Checking if the calling worker is needed to meet the requirements of the queue.
        if (GetSignedConcurrencyRequest(counter) <= 0.0f)
            return;

        // If the caller is needed then we need to update the concurrency of the queue
    } while(!m_concurrency.compare_exchange_weak(counter.value, counter.value + one_current.value, std::memory_order_acq_rel));

    if constexpr (RUKEN_TRACE_SHOW_WORKER_ZONES)
    {
        TracyCPlotF(g_request, GetSignedConcurrencyRequest(counter))
        TracyCPlotI(g_current, counter.current_concurrency)
        TracyCPlotI(g_optimal, counter.optimal_concurrency)
    }

    do
    {
        // Inner loop consumes jobs and checks if the queue still needs us.
        if (!in_greedy)
        {
            WorkerInfo::remaining_tasks = 1;
            TryConsumeJob(50);
        }

        else while (GetSignedConcurrencyRequest(counter, -1) > 0.0F && !in_stop_token.stop_requested())
        {
            // Consuming a maximum of 10 jobs before checking if we are still needed
            WorkerInfo::remaining_tasks = 10;
            while (WorkerInfo::remaining_tasks-- > 1 && !in_stop_token.stop_requested())
                TryConsumeJob(50);

            // Checking if the queue still needs us
            counter.value = m_concurrency.load(std::memory_order_acquire);

            if constexpr (RUKEN_TRACE_SHOW_WORKER_ZONES)
            {
                TracyCPlotF(g_request, GetSignedConcurrencyRequest(counter))
                TracyCPlotI(g_current, counter.current_concurrency)
                TracyCPlotI(g_optimal, counter.optimal_concurrency)
            }
        }

    // The outer loop makes sure only one thread exits the queue at the same time to avoid overshooting requests.
    } while(!m_concurrency.compare_exchange_weak(counter.value,
        counter.value - one_current.value, std::memory_order_acq_rel
    ) && !in_stop_token.stop_requested());
}

RkFloat JobQueue::ComputeOptimalConcurrency(RkUint32 const in_max_concurrency) const noexcept
{
    // For now the optimal concurrency is just the max concurrency
    return static_cast<RkFloat>(in_max_concurrency);
}