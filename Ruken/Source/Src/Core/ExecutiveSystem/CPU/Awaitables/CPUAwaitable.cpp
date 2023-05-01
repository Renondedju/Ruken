#include <atomic>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

USING_RUKEN_NAMESPACE

RkVoid CPUAwaitable::SignalCompletion() noexcept
{
    CPUSuspension::Node* selection {std::addressof(m_suspension_node)};
    CPUSuspension*       value;

    while(true)
    {
        do
        {
            value = nullptr; // Checking if we have awaiters to signal
            if (selection->compare_exchange_strong(value, CPUSuspension::consumed, std::memory_order_release, std::memory_order_acquire))
                return;

        // And waiting for any lock in the process
        } while(value == CPUSuspension::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the awaiter 
        if (selection->compare_exchange_weak(value, CPUSuspension::consumed, std::memory_order_release, std::memory_order_acquire))
        {
            // Fetching the next awaiter before notifying the dependent event in case it gets destroyed as a side effect
            CPUSuspension::Node* next = &value->next;
            value->OnCompletion();

            selection = next;
        }
    }
}

RkVoid CPUAwaitable::Reset() noexcept
{
    m_suspension_node.store(nullptr, std::memory_order_release);
}

RkBool CPUAwaitable::Completed() const noexcept
{
    return m_suspension_node.load(std::memory_order_acquire) == CPUSuspension::consumed;
}

CPUSuspension::Node& CPUAwaitable::GetSuspensionNode() noexcept
{
    return m_suspension_node;
}