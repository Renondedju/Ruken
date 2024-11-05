#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"

#include "Meta/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TValue>
CPUAwaitable<TValue>::CPUAwaitable(CPUAwaiterList<TValue>& in_list_head, TValue* in_value) noexcept requires (has_value):
    CPUAwaitableStorage<TValue*> {in_value},
    m_awaiter_list               {std::addressof(in_list_head)}
{
    RUKEN_ASSERT(in_value != nullptr,
        "A pointer is used instead of a reference to avoid having to specialize the whole class for void."
        "You are expected to always provide a valid pointer.");
}

template<typename TValue>
CPUAwaitable<TValue>::CPUAwaitable(CPUAwaiterList<TValue>& in_list_head) noexcept requires (!has_value):
    CPUAwaitableStorage<TValue*> {},
    m_awaiter_list               {std::addressof(in_list_head)}
{}

// --- Signal

template <typename TValue>
template <typename TOtherValue>
RkBool CPUAwaitable<TValue>::TryAttach(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept
{
    // FIXME: Make a proper type erasure
    CPUAwaiter<RkVoid>& awaiter {reinterpret_cast<CPUAwaiter<RkVoid>&>(in_awaiter)};
    awaiter.head = reinterpret_cast<CPUAwaiterList<RkVoid>*>(m_awaiter_list);

    // ---
    CPUAwaiter<RkVoid>* head_value {(awaiter.head->load(std::memory_order_acquire))};

    do
    {
        while (head_value == CPUAwaiter<RkVoid>::locked) // If the current value is locked we need to wait
            head_value = awaiter.head->load(std::memory_order_acquire);
*
        // Checking if the event signaled a completion in the meanwhile
        if (head_value == CPUAwaiter<RkVoid>::consumed)
            return false;

        // Update linked list to point at the current head
        awaiter.next.store(head_value, std::memory_order_release);

        // Finally, if the head we originally fetched is still the actual head
        // (it could have been signaled, locked or swapped while we were testing stuff)
        // swapping the old list head with this awaiter as the new list head.
    } while (!awaiter.head->compare_exchange_weak(head_value, std::addressof(awaiter),
                                          std::memory_order_release,
                                          std::memory_order_acquire));

    // Operation succeeded
    return true;
}

template <typename TValue>
template <typename TOtherValue>
RkBool CPUAwaitable<TValue>::TryDetach(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept
{
    // FIXME: Make a proper type erasure
    CPUAwaiter<RkVoid>& awaiter {reinterpret_cast<CPUAwaiter<RkVoid>&>(in_awaiter)};

    RUKEN_ASSERT(reinterpret_cast<CPUAwaiterList<RkVoid>*>(m_awaiter_list) == awaiter.head,
        "Awaiter must have been attached to this awaiable to be detached");

    // If the awaiter hasn't been completed in due time,
    // we need to detach it from the awaited event to cancel
    // our wait without crashing later down the line
    if (awaiter.next.load(std::memory_order_acquire) == CPUAwaiter<RkVoid>::consumed)
        return false;

    // Attempting to detach from the awaited event by looking for our
    // address though the list of suspensions
    CPUAwaiterList<RkVoid>* selection {awaiter.head};
    CPUAwaiter    <RkVoid>* expected  {std::addressof(awaiter)};

    // If this awaiter is the one we were looking for, then we lock it to ensure nobody swaps our `next` pointer
    while(!selection->compare_exchange_strong(expected, CPUAwaiter<RkVoid>::locked, std::memory_order_acq_rel, std::memory_order_acquire))
    {
        // Otherwise we need to check if the event hasn't been signaled in the meantime
        // or if we haven't found ourselves in the list for some reason
        if (expected == CPUAwaiter<RkVoid>::consumed ||
            expected == nullptr)
            return false;

        // And if the selection isn't currently locked, then we can finally test the next awaiter in the list
        // otherwise we'll just retry until the lock has been released
        if (expected != CPUAwaiter<RkVoid>::locked)
            selection = &expected->next;

        expected = std::addressof(awaiter);
    }

    // Lock acquired, we can now safely read the next pointer
    // and swap our lock with that, effectively releasing our lock
    selection->store(awaiter.next.load(std::memory_order_acquire), std::memory_order_release);

    return true;
}

template <typename TValue>
template <typename TOtherValue>
requires (std::is_void_v<TOtherValue> || std::is_same_v<TOtherValue, TValue>)
RkVoid CPUAwaitable<TValue>::AttachOrSignal(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept
{
    if (TryAttach(in_awaiter))
        return;

    if constexpr (!std::is_void_v<TOtherValue>)
        in_awaiter.signal.Signal(this->value);
    else
        in_awaiter.signal.Signal();
}

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal(TValue* in_value) noexcept requires (has_value)
{ this->value = in_value; DoSignal(); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal() noexcept requires (!has_value)
{ DoSignal(); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::DoSignal() noexcept
{
    RUKEN_ASSERT(m_awaiter_list != nullptr, "");

    CPUAwaiterList<TValue>* selection    {m_awaiter_list};
    CPUAwaiter<TValue>*     previous     {nullptr};
    CPUAwaiter<TValue>*		continuation;

    while (true)
    {
        // Waiting for any locks
        while ((continuation = selection->load(std::memory_order_acquire)) == CPUAwaiter<TValue>::locked)
            ;

        // We can signal the previous awaiter.
        // This is done this way in case it gets destroyed as a side effect
        // and avoids us to read potentially unallocated memory.
        if (previous)
            if constexpr (!std::is_void_v<TValue>)
                previous->signal.Signal(this->value);
            else
                previous->signal.Signal();

        // If the continuation is null we reached the end of the chain
        if (continuation == nullptr)
            return;

        // Fetching the next awaiter
        selection = &continuation->next;
        previous   = continuation;
    }
}

// --- Consume

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Consume(RkBool const in_signal, TValue* in_value) noexcept requires (has_value)
{ if (in_signal) this->value = in_value; DoConsume(in_signal); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Consume(RkBool const in_signal) noexcept requires (!has_value)
{ DoConsume(in_signal); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::DoConsume(RkBool const in_signal) noexcept
{
    RUKEN_ASSERT(m_awaiter_list != nullptr, "");

    CPUAwaiterList<TValue>* selection    {m_awaiter_list};
    CPUAwaiter<TValue>*   	previous     {nullptr};
    CPUAwaiter<TValue>*   	continuation;

    if (selection->load(std::memory_order_acquire) == CPUAwaiter<TValue>::consumed)
        return;

    while(true)
    {
        do
        {
            // Checking for continuations to consume
            if (selection->compare_exchange_strong(continuation = nullptr, CPUAwaiter<TValue>::consumed, std::memory_order_acq_rel))
            {
                // If there is not we need to notify the
                // last awaiter before returning.
                if (in_signal && previous)
                    if constexpr (!std::is_void_v<TValue>)
                        previous->signal.Signal(this->value);
                    else
                        previous->signal.Signal();

                return;
            }

        // If there are, waiting for any lock in the process
        } while(continuation == CPUAwaiter<TValue>::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the previous awaiter
        if (selection->compare_exchange_weak(continuation, CPUAwaiter<TValue>::consumed, std::memory_order_acq_rel))
        {
            // This is done this way in case it gets destroyed as a side effect
            // and avoids us to read potentially unallocated memory in the code above.
            if (in_signal && previous)
                if constexpr (!std::is_void_v<TValue>)
                    previous->signal.Signal(this->value);
                else
                    previous->signal.Signal();

            // Fetching the next awaiter
            selection = &continuation->next;
            previous   = continuation;
        }
    }
}

// ---

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Reset() noexcept
{
    m_awaiter_list->store(nullptr, std::memory_order_release);
}

template <typename TValue>
RkBool CPUAwaitable<TValue>::Completed() const noexcept
{
    return m_awaiter_list->load(std::memory_order_acquire) == CPUAwaiter<TValue>::consumed;
}

END_RUKEN_NAMESPACE
