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
requires (std::is_void_v<TOtherValue> || std::is_same_v<TOtherValue, TValue>)
RkVoid CPUAwaitable<TValue>::AttachOrSignal(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept
{
    if (in_awaiter.TryAttach())
        return;

    if constexpr (!std::is_void_v<TOtherValue>)
        in_awaiter.signal.Signal(this->value);
    else
        in_awaiter.signal.Signal();
}

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal(TValue* in_value, RkUint64* in_tag) noexcept requires (has_value)
{ this->value = in_value; DoSignal(in_tag); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal(RkUint64* in_tag) noexcept requires (!has_value)
{ DoSignal(in_tag); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::DoSignal(RkUint64* in_tag) noexcept
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
        if (previous && (in_tag == nullptr || previous->tag == *in_tag))
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
RkVoid CPUAwaitable<TValue>::Consume(RkBool const in_signal, TValue* in_value, RkUint64* in_tag) noexcept requires (has_value)
{
    if (in_signal)
        this->value = in_value;

    DoConsume(in_signal, in_tag);
}

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Consume(RkBool const in_signal, RkUint64* in_tag) noexcept requires (!has_value)
{
    DoConsume(in_signal, in_tag);
}

template <typename TValue>
RkVoid CPUAwaitable<TValue>::DoConsume(RkBool const in_signal, RkUint64* in_tag) noexcept
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
            if (selection->compare_exchange_strong(continuation = nullptr, CPUAwaiter<TValue>::consumed, std::memory_order_acq_rel)
                || (in_tag && continuation != CPUAwaiter<TValue>::locked && continuation->tag != *in_tag))
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

template<typename TValue>
CPUAwaiter<TValue> CPUAwaitable<TValue>::operator co_await() const noexcept
{
    return CPUAwaiter<TValue>(m_awaiter_list);
}

template <typename TValue>
RkBool CPUAwaitable<TValue>::Completed() const noexcept
{
    return m_awaiter_list->load(std::memory_order_acquire) == CPUAwaiter<TValue>::consumed;
}

END_RUKEN_NAMESPACE
