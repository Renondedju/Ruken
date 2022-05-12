#pragma once

template <ProcessingQueueType TQueue> requires (TQueue::instruction_type == EInstructionType::Direct)
RkVoid AsynchronousEvent<TQueue>::SignalCompletion() noexcept
{
    DirectSubscriptionBase::Node* selection {std::addressof(m_head)};
    DirectSubscriptionBase*       value;

    while(true)
    {
        do
        {
            value = nullptr; // Checking if we have awaiters to signal
            if (selection->compare_exchange_strong(value, DirectSubscriptionBase::completed, std::memory_order_release, std::memory_order_acquire))
                return;

        // And waiting for any lock in the process
        } while(value == DirectSubscriptionBase::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the awaiter 
        if (selection->compare_exchange_weak(value, DirectSubscriptionBase::completed, std::memory_order_release, std::memory_order_acquire))
        {
            // Fetching the next awaiter before notifying the dependent event in case it gets destroyed as a side effect
            DirectSubscriptionBase::Node* next = &value->m_next;
            
            value->OnCompletion();
            selection = next;
        }
    }
}

template <ProcessingQueueType TQueue> requires (TQueue::instruction_type == EInstructionType::Direct)
RkVoid AsynchronousEvent<TQueue>::Reset() noexcept
{
    m_head.store(nullptr, std::memory_order_release);
}

template <ProcessingQueueType TQueue> requires (TQueue::instruction_type == EInstructionType::Direct)
RkBool AsynchronousEvent<TQueue>::Completed() const noexcept
{
    return m_head.load(std::memory_order_acquire) == DirectSubscriptionBase::completed;
}