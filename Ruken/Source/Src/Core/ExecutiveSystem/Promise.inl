#pragma once

#pragma region Methods

template <QueueHandleType TQueueHandle>
Task<TQueueHandle> Promise<TQueueHandle>::get_return_object() noexcept
{
    auto handle = std::coroutine_handle<Promise<TQueueHandle>>::from_promise(*this);

    // If the coroutine contains direct instructions, then
    // we let the queue do it's things with the execution, otherwise the handle is executed in place
    if constexpr (typename ProcessingUnit::instruction_type == EInstructionType::Direct)
        TQueueHandle::queue.Push(handle);

    return {handle};
}

template <QueueHandleType TQueueHandle>
RkVoid Promise<TQueueHandle>::unhandled_exception()
{
    std::rethrow_exception(std::current_exception());
}

template <QueueHandleType TQueueHandle>
typename Promise<TQueueHandle>::FinalSuspension Promise<TQueueHandle>::final_suspend() noexcept
{
    if constexpr (typename ProcessingUnit::instruction_type == EInstructionType::Direct)
        this->SignalCompletion();

    return {};
}

#pragma endregion
