#pragma once

#include <atomic>

#include "Build/Attributes/EmptyBases.hpp"

#include "Core/ExecutiveSystem/Awaitable.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUAwaitableResult.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
class CPUAwaitableHandle;

/**
 * \brief This class acts as a base for every CPU awaitable.
 *
 * \note
 * Continuations will attempt to detach themselves upon
 * destruction if the event has not been consumed.
 * If an awaitable can be destroyed before all of it's attached
 * continuations, you must make sure it is consumed either by calling
 * CPUAwaitable::Consume() or CPUAwaitable::SignalConsume().
 */
template <typename TResult>
class RUKEN_EMPTY_BASES CPUAwaitable:
    public CPUAwaitableResult<TResult>,
    public Awaitable<CentralProcessingUnit, TResult>
{
    friend CPUContinuation;     // For the continuation hook
    friend CPUAwaitableHandle; // For the reference count

    #pragma region Members

    CPUContinuation::Node  m_continuation_hook {nullptr};
    std::atomic<RkSize>    m_references        {1ULL};

    #pragma endregion

    protected:

        #pragma region Methods

        /**
         * \brief Signals the completion to all of the attached awaiters.
         * Reset() does not need to be called and attached continuations are not consumed.
         */
        RkVoid Signal() const noexcept;

        /**
         * \brief Consumes the attached continuations.
         */
        RkVoid Consume() noexcept;

        /**
         * \brief Signals the completion of the event and consumes all the attached continuations.
         * The awaitable will become completed until Reset() has been called.
         */
        RkVoid SignalConsume() noexcept;

        /**
         * \brief Resets the event, making it non completed and ready to hook new awaiters.
         *
         * Do note that any awaiting event will be lost and won't resume ever if reset is called when
         * CPUAwaitable::Completed() returns false. Make sure to signal a completion before resetting.
         */
        RkVoid Reset() noexcept;

        virtual RkVoid Deallocate() = 0;

        #pragma endregion

    public:

        #pragma region Lifetime

        CPUAwaitable()                    = default;
        CPUAwaitable(CPUAwaitable const&) = delete;
        CPUAwaitable(CPUAwaitable&&     ) = delete;
        virtual ~CPUAwaitable()           = default;

        CPUAwaitable& operator=(CPUAwaitable const&) = delete;
        CPUAwaitable& operator=(CPUAwaitable&&     ) = delete;

        #pragma endregion

        #pragma region Methods

        RkVoid DecrementReferenceCount() noexcept;
        RkVoid IncrementReferenceCount() noexcept;

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        [[nodiscard]]
		RkBool Completed() const noexcept;

        #pragma endregion
};

#pragma region Implementation

template <typename TResult>
RkVoid CPUAwaitable<TResult>::Signal() const noexcept
{
    CPUContinuation::Node const* selection    {std::addressof(m_continuation_hook)};
    CPUAwaiter*                  awaiter      {nullptr};
    CPUContinuation*             continuation;

    while (true)
    {
        // Waiting for any locks
        while ((continuation = selection->load(std::memory_order_acquire)) == CPUContinuation::locked)
            ;

        // If the continuation is null we reached the end of the chain
        if (continuation == nullptr)
        {
            if (awaiter) awaiter->OnAwaitedContinuation();
            return;
        }

        // Otherwise we can signal the previous awaiter.
        // This is done this way in case it gets destroyed as a side effect
        // and avoids us to read potentially unallocated memory in the code above.
        if (awaiter) awaiter->OnAwaitedContinuation();

        // Fetching the next awaiter
        selection = &continuation->next;
        awaiter   =  continuation->owner;
    }
}

template <typename TResult>
RkVoid CPUAwaitable<TResult>::Consume() noexcept
{
    CPUContinuation::Node* selection    {std::addressof(m_continuation_hook)};
    CPUContinuation*       continuation {nullptr};

    while(true)
    {
        do
        {
            // Checking for continuations to consume
            if (selection->compare_exchange_strong(continuation = nullptr, CPUContinuation::consumed, std::memory_order_acq_rel))
                return;

        // If there are, waiting for any lock in the process
        } while(continuation == CPUContinuation::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the previous awaiter 
        if (selection->compare_exchange_weak(continuation, CPUContinuation::consumed, std::memory_order_acq_rel))
            selection = &continuation->next; // Fetching the next awaiter
    }
}

template <typename TResult>
RkVoid CPUAwaitable<TResult>::SignalConsume() noexcept
{
    CPUContinuation::Node* selection    {std::addressof(m_continuation_hook)};
    CPUContinuation*       continuation {nullptr};
    CPUAwaiter*            awaiter      {nullptr};

    while(true)
    {
        do
        {
            // Checking for continuations to consume
            if (selection->compare_exchange_strong(continuation = nullptr, CPUContinuation::consumed, std::memory_order_acq_rel))
            {
                // If there is not we need to notify the
                // last awaiter before returning.
                if (awaiter)
                    awaiter->OnAwaitedContinuation();

                return;
            }

        // If there are, waiting for any lock in the process
        } while(continuation == CPUContinuation::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the previous awaiter 
        if (selection->compare_exchange_weak(continuation, CPUContinuation::consumed, std::memory_order_acq_rel))
        {
            // This is done this way in case it gets destroyed as a side effect
            // and avoids us to read potentially unallocated memory in the code above.
            if (awaiter)
                awaiter->OnAwaitedContinuation();

            // Fetching the next awaiter
            selection = &continuation->next;
            awaiter   =  continuation->owner;
        }
    }
}

template <typename TResult>
RkVoid CPUAwaitable<TResult>::Reset() noexcept
{
    m_continuation_hook.store(nullptr, std::memory_order_release);
}

template <typename TResult>
RkVoid CPUAwaitable<TResult>::DecrementReferenceCount() noexcept
{
    if (m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
        Deallocate();
}

template <typename TResult>
RkVoid CPUAwaitable<TResult>::IncrementReferenceCount() noexcept
{
    m_references.fetch_add(1, std::memory_order_acq_rel);
}

template <typename TResult>
RkBool CPUAwaitable<TResult>::Completed() const noexcept
{
    return m_continuation_hook.load(std::memory_order_acquire) == CPUContinuation::consumed;
}

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl"

#pragma endregion

END_RUKEN_NAMESPACE