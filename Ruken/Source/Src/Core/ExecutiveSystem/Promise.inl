#pragma once

#pragma region Methods

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

#pragma endregion