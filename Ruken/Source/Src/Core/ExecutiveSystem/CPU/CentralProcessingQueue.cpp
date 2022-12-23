#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

CentralProcessingQueue::CentralProcessingQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{}

RkVoid CentralProcessingQueue::TryConsumeJob(ConcurrencyCounter& out_new_concurrency, RkUint32 const in_max_attempts) noexcept
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
    out_new_concurrency.value = m_concurrency.fetch_sub(one_optimal.value, std::memory_order_release);
    job.resume();
}

RkFloat CentralProcessingQueue::GetSignedConcurrencyRequest(ConcurrencyCounter const& in_concurrency, RkUint32 const in_offset) const noexcept
{
    return OptimalConcurrency(in_concurrency.optimal_concurrency) - static_cast<RkFloat>(in_concurrency.current_concurrency + in_offset);
}

RkVoid CentralProcessingQueue::Push(std::coroutine_handle<>&& in_handle) noexcept
{
    ConcurrencyCounter constexpr one_optimal { {.current_concurrency = 0, .optimal_concurrency = 1} };

    while(!m_queue.try_push(in_handle))
        atomic_queue::spin_loop_pause();

    m_concurrency.fetch_add(one_optimal.value, std::memory_order_release);
}

RkVoid CentralProcessingQueue::PopAndRun(RkBool const in_sticky, std::stop_token const& in_stop_token) noexcept
{
    RkFloat                      signed_request;
    ConcurrencyCounter           counter     { .value = m_concurrency.load(std::memory_order_relaxed) };
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
        TryConsumeJob(counter, 50);

    else do
    {
        // Otherwise, we'll consume a maximum of 10 jobs
        for(int i = 0; i < 10; ++i)
            TryConsumeJob(counter, 50);

        // Checking if the queue still needs us
        signed_request = GetSignedConcurrencyRequest(counter, -1);
    } while (signed_request >= 1.0F && !in_stop_token.stop_requested());

    // Finally decrementing the current concurrency of the queue
    m_concurrency.fetch_sub(one_current.value, std::memory_order_release);
}

RkBool CentralProcessingQueue::Empty() const noexcept
{
    return m_queue.was_empty();
}

RkFloat CentralProcessingQueue::OptimalConcurrency(RkUint32 const in_max_concurrency) const noexcept
{
    return static_cast<RkFloat>(in_max_concurrency);
}