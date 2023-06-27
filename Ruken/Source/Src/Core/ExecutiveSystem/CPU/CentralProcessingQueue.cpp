#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

CentralProcessingQueue::CentralProcessingQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{}

RkVoid CentralProcessingQueue::TryConsumeJob(RkUint32 const in_max_attempts) noexcept
{
    std::coroutine_handle<>      job;
    ConcurrencyCounter constexpr one_optimal { {.current_concurrency = 0, .optimal_concurrency = 1} };
    RkSize                       remaining_attempts { static_cast<RkSize>(in_max_attempts) + 1ULL   };

    // Attempting to pop a job
    while (--remaining_attempts > 0 && !m_queue.try_pop(job))
        atomic_queue::spin_loop_pause();

    // Escaping timeouts
    if (remaining_attempts == 0)
        return;

    // Otherwise we need to update the concurrency and run the job
    m_concurrency.fetch_sub(one_optimal.value, std::memory_order_acq_rel);
    job.resume();
}

RkFloat CentralProcessingQueue::GetSignedConcurrencyRequest(ConcurrencyCounter const& in_concurrency, RkUint32 const in_offset) const noexcept
{
    return ComputeOptimalConcurrency(in_concurrency.optimal_concurrency)
              - static_cast<RkFloat>(in_concurrency.current_concurrency)
              + static_cast<RkFloat>(in_offset);
}

RkVoid CentralProcessingQueue::Push(std::coroutine_handle<>&& in_handle) noexcept
{
    ConcurrencyCounter constexpr one_optimal { {.current_concurrency = 0, .optimal_concurrency = 1} };

    m_queue      .push     (in_handle);
    m_concurrency.fetch_add(one_optimal.value, std::memory_order_acq_rel);
}

RkVoid CentralProcessingQueue::PopAndRun(RkBool const in_sticky, std::stop_token const& in_stop_token) noexcept
{
    RkFloat                      signed_request;
    ConcurrencyCounter           counter     { .value = m_concurrency.load(std::memory_order_acquire) };
    ConcurrencyCounter constexpr one_current { {.current_concurrency = 1, .optimal_concurrency = 0} };

    do
    {
        // Checking if the calling worker is needed to meet the requirements of the queue.
        if ((signed_request = GetSignedConcurrencyRequest(counter, 1)) < 0.0F)
            return;

        // If the caller is needed then we need to update the concurrency of the queue
    } while(!m_concurrency.compare_exchange_weak(counter.value, counter.value + one_current.value, std::memory_order_acq_rel));

    // If the caller don't want to stick to the queue
    // then we only try to consume a single job before returning
    if (!in_sticky)
        TryConsumeJob(50);

    else do
    {
        // Otherwise, we'll consume a maximum of 10 jobs
        for(int i = 0; i < 10; ++i)
            TryConsumeJob(50);

        // Checking if the queue still needs us
        counter.value  = m_concurrency.load(std::memory_order_acquire);
        signed_request = GetSignedConcurrencyRequest(counter, -1);
    } while (signed_request >= 1.0F && !in_stop_token.stop_requested());

    // Finally decrementing the current concurrency of the queue
    m_concurrency.fetch_sub(one_current.value, std::memory_order_acq_rel);
}

ConcurrencyCounter CentralProcessingQueue::GetConcurrencyCounter() const noexcept
{
    ConcurrencyCounter const counter { .value = m_concurrency.load(std::memory_order_relaxed) };
    return counter;
}

RkFloat CentralProcessingQueue::ComputeOptimalConcurrency(RkUint32 const in_max_concurrency) const noexcept
{
    // For now the optimal concurrency is just the max concurrency
    return static_cast<RkFloat>(in_max_concurrency);
}