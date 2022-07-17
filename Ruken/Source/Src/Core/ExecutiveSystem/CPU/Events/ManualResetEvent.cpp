#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

RkVoid ManualResetEvent::SignalCompletion() noexcept
{
    CPUSubscription::Node* selection {std::addressof(m_head)};
    CPUSubscription*       value;

    while(true)
    {
        do
        {
            value = nullptr; // Checking if we have awaiters to signal
            if (selection->compare_exchange_strong(value, CPUSubscription::completed, std::memory_order_release, std::memory_order_acquire))
                return;

        // And waiting for any lock in the process
        } while(value == CPUSubscription::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the awaiter 
        if (selection->compare_exchange_weak(value, CPUSubscription::completed, std::memory_order_release, std::memory_order_acquire))
        {
            // Fetching the next awaiter before notifying the dependent event in case it gets destroyed as a side effect
            CPUSubscription::Node* next = &value->next;
            value->OnCompletion();

            selection = next;
        }
    }
}

RkVoid ManualResetEvent::Reset() noexcept
{
    m_head.store(nullptr, std::memory_order_release);
}

RkBool ManualResetEvent::Completed() const noexcept
{
    return m_head.load(std::memory_order_acquire) == CPUSubscription::completed;
}
