#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"

#include "Meta/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TValue>
CPUAwaitable<TValue>::CPUAwaitable(CPUContinuationNodePtr& in_continuation_hook, TValue* in_value) noexcept requires (has_value):
    CPUAwaitableStorage<ValueT*> {in_value},
    m_continuation_hook          {std::addressof(in_continuation_hook)}
{
    RUKEN_ASSERT(in_value != nullptr,
        "A pointer is used instead of a reference to avoid having to specialize the whole class for void."
        "You are expected to always provide a valid pointer.");
}

template<typename TValue>
CPUAwaitable<TValue>::CPUAwaitable(CPUContinuationNodePtr& in_continuation_hook) noexcept requires (!has_value):
    CPUAwaitableStorage<ValueT*> {},
    m_continuation_hook          {std::addressof(in_continuation_hook)}
{}

// --- Signal

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal(TValue* in_value) noexcept requires (has_value)
{ this->value = in_value; DoSignal(); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::Signal() noexcept requires (!has_value)
{ DoSignal(); }

template <typename TValue>
RkVoid CPUAwaitable<TValue>::DoSignal() noexcept
{
    RUKEN_ASSERT(m_continuation_hook != nullptr, "");

    std::atomic<CPUContinuationNode*> const* selection    {m_continuation_hook};
    CPUContinuationNode*                     previous     {nullptr};
    CPUContinuationNode*		             continuation;

    while (true)
    {
        // Waiting for any locks
        while ((continuation = selection->load(std::memory_order_acquire)) == CPUContinuationNode::locked)
            ;

        // We can signal the previous awaiter.
        // This is done this way in case it gets destroyed as a side effect
        // and avoids us to read potentially unallocated memory.
        if (previous)
            if constexpr (!std::is_same_v<ValueT, RkVoid>)
                reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal(this->value);
            else
                reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal();

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
    RUKEN_ASSERT(m_continuation_hook != nullptr, "");

    std::atomic<CPUContinuationNode*>* selection    {m_continuation_hook};
    CPUContinuationNode*         	   previous     {nullptr};
    CPUContinuationNode*         	   continuation;

    if (selection->load(std::memory_order_acquire) == CPUContinuationNode::consumed)
        return;

    while(true)
    {
        do
        {
            // Checking for continuations to consume
            if (selection->compare_exchange_strong(continuation = nullptr, CPUContinuationNode::consumed, std::memory_order_acq_rel))
            {
                // If there is not we need to notify the
                // last awaiter before returning.
                if (in_signal && previous)
                    if constexpr (!std::is_same_v<TValue, RkVoid>)
                        reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal(this->value);
                    else
                        reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal();

                return;
            }

        // If there are, waiting for any lock in the process
        } while(continuation == CPUContinuationNode::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the previous awaiter
        if (selection->compare_exchange_weak(continuation, CPUContinuationNode::consumed, std::memory_order_acq_rel))
        {
            // This is done this way in case it gets destroyed as a side effect
            // and avoids us to read potentially unallocated memory in the code above.
            if (in_signal && previous)
                if constexpr (!std::is_same_v<TValue, RkVoid>)
                    reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal(this->value);
                else
                    reinterpret_cast<CPUContinuation<TValue>*>(previous)->Signal();

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
    m_continuation_hook->store(nullptr, std::memory_order_release);
}

template <typename TValue>
RkBool CPUAwaitable<TValue>::Completed() const noexcept
{
    return m_continuation_hook->load(std::memory_order_acquire) == CPUContinuation<TValue>::consumed;
}

template<typename TValue>
CPUContinuationNodePtr* CPUAwaitable<TValue>::GetContinuationHook() const noexcept
{
    return m_continuation_hook;
}

END_RUKEN_NAMESPACE
