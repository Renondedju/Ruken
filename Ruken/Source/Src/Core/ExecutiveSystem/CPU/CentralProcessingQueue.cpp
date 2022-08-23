#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

CentralProcessingQueue::CentralProcessingQueue(const RkSize in_size) noexcept:
	m_queue {static_cast<unsigned>(in_size)}
{}

RkVoid CentralProcessingQueue::Enqueue(std::coroutine_handle<>&& in_handle) noexcept
{
    while(!m_queue.try_push(in_handle))
        atomic_queue::spin_loop_pause();
}

RkBool CentralProcessingQueue::TryDequeue(std::coroutine_handle<>& out_handle, RkSize const in_timeout) noexcept
{
    RkSize remaining_attempts {in_timeout + 1ULL};

    while(!m_queue.try_pop(out_handle) && --remaining_attempts != 0ULL)
        atomic_queue::spin_loop_pause();

    return remaining_attempts;
}

RkBool CentralProcessingQueue::Empty() const noexcept
{
    return m_queue.was_empty();
}