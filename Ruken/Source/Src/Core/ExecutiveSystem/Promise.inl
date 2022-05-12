#pragma once

#pragma region Methods

template <ProcessingQueueType TQueue>
template <ProcessingQueueType TAwaitedQueue> requires AwaiterType<Subscription<TQueue, TAwaitedQueue>>
Subscription<TQueue, TAwaitedQueue> Promise<TQueue>::await_transform(AsynchronousEvent<TAwaitedQueue>& in_awaited_event) noexcept
{
    return Subscription<TQueue, TAwaitedQueue>({std::coroutine_handle<Promise<TQueue>>::from_promise(*this)}, in_awaited_event);
}

template <ProcessingQueueType TQueue>
template <ProcessingQueueType TAwaitedQueue> requires AwaiterType<Subscription<TQueue, TAwaitedQueue>>
Subscription<TQueue, TAwaitedQueue> Promise<TQueue>::await_transform(Task<TAwaitedQueue> const& in_awaited_task) noexcept
{
    return Subscription<TQueue, TAwaitedQueue>({std::coroutine_handle<Promise<TQueue>>::from_promise(*this)}, in_awaited_task.promise());
}

template <ProcessingQueueType TQueue>
Task<TQueue> Promise<TQueue>::get_return_object() noexcept
{
    auto handle = std::coroutine_handle<Promise<TQueue>>::from_promise(*this);

    // If the coroutine contains direct instructions, then
    // we let the queue do it's things with the execution, otherwise the handle is executed in place
    if constexpr (typename TQueue::ProcessingUnit::instruction_type == EInstructionType::Direct)
        TQueue::Enqueue(handle);

    return {handle};
}

template <ProcessingQueueType TQueue>
RkVoid Promise<TQueue>::unhandled_exception()
{
    std::rethrow_exception(std::current_exception());
}

template <ProcessingQueueType TQueue>
typename Promise<TQueue>::FinalSuspension Promise<TQueue>::final_suspend() noexcept
{
    if constexpr (typename TQueue::ProcessingUnit::instruction_type == EInstructionType::Direct)
        this->SignalCompletion();

    return {};
}

#pragma endregion
