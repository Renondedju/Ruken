#pragma once

template <typename TInheriting, RkSize TSize>
RkVoid CentralProcessingQueue<TInheriting, TSize>::Enqueue(std::coroutine_handle<>&& in_handle) noexcept
{
    while(!m_queue.try_push(in_handle))
        atomic_queue::spin_loop_pause();

    in_handle.resume();
}

template <typename TInheriting, RkSize TSize>
RkBool CentralProcessingQueue<TInheriting, TSize>::TryDequeue(std::coroutine_handle<>& out_handle, RkSize const in_timeout) noexcept
{
    RkSize remaining_attempts {in_timeout + 1ULL};

    while(!m_queue.try_pop(out_handle) && --remaining_attempts != 0ULL)
        atomic_queue::spin_loop_pause();

    return remaining_attempts;
}

template <typename TInheriting, RkSize TSize>
RkBool CentralProcessingQueue<TInheriting, TSize>::Empty() noexcept
{
    return m_queue.was_empty();
}