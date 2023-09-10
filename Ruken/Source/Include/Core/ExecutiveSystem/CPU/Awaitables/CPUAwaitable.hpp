#pragma once

#include <atomic>

#include "Build/Attributes/EmptyBases.hpp"

#include "Core/ExecutiveSystem/Awaitable.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUReturningAwaitableBase.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TReturnType>
class CPUAwaitableHandle;

/**
 * \brief This class acts as a base for every CPU awaitable
 */
template <typename TReturnType>
class RUKEN_EMPTY_BASES CPUAwaitable:
    protected CPUReturningAwaitableBase,
    public    Awaitable<CentralProcessingUnit, TReturnType>
{
    friend CPUAwaitableHandle<TReturnType>;
    using ContinuationHookType = CPUContinuation::Node;
    public:
    #pragma region Members

    ContinuationHookType  m_continuation_hook {nullptr};
    std::atomic<RkSize>   m_references        {0ULL};

    #pragma endregion

    protected:
    public:

        #pragma region Members

        TReturnType m_result;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Reads the reference counter of the awaitable.
         *        This method will wait for any incoherent write to the counter before fetching the value.
         * \return Number of references to the awaitable
         */
        [[nodiscard]]
        RkSize GetReferenceCount() const noexcept;

        #pragma endregion

    public:

        #pragma region Lifetime

        CPUAwaitable()                    = default;
        CPUAwaitable(CPUAwaitable const&) = delete;
        CPUAwaitable(CPUAwaitable&&     ) = delete;
        ~CPUAwaitable()          override = default;

        CPUAwaitable& operator=(CPUAwaitable const&) = delete;
        CPUAwaitable& operator=(CPUAwaitable&&     ) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Signals the completion of the event to every of the attached awaiters.
         * This will consume the event and make it completed until Reset() has been called.
         */
        RkVoid SignalCompletion() noexcept;

        /**
         * \brief Resets the event, making it non completed and ready to hook new awaiters.
         * Do note that any awaiting event will be lost and won't resume ever if reset is called when
         * Completed() returns false. Make sure to signal a completion before resetting.
         */
        RkVoid Reset() noexcept;

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        [[nodiscard]]
		RkBool Completed() const noexcept;

        #pragma endregion
};

/**
 * \brief This class acts as a base for every CPU awaitable
 */
template <>
class RUKEN_EMPTY_BASES CPUAwaitable<RkVoid>:
    protected CPUReturningAwaitableBase,
    public    Awaitable<CentralProcessingUnit, RkVoid>
{
    friend CPUAwaitableHandle<RkVoid>;
    using ContinuationHookType = CPUContinuation::Node;
    public:
    #pragma region Members

    ContinuationHookType m_continuation_hook {nullptr};

    #pragma endregion

    protected:

        #pragma region Methods

        /**
         * \brief Signals the completion of the event to every of the attached awaiters.
         * This will consume the event and make it completed until Reset() has been called.
         */
        RkVoid SignalCompletion() noexcept;

        /**
         * \brief Resets the event, making it non completed and ready to hook new awaiters.
         * Do note that any awaiting event will be lost and won't resume ever if reset is called when
         * Completed() returns false. Make sure to signal a completion before resetting.
         */
        RkVoid Reset() noexcept;

        #pragma endregion

    public:

        #pragma region Lifetime

        CPUAwaitable()                    = default;
        CPUAwaitable(CPUAwaitable const&) = delete;
        CPUAwaitable(CPUAwaitable&&     ) = delete;
        ~CPUAwaitable()          override = default;

        CPUAwaitable& operator=(CPUAwaitable const&) = delete;
        CPUAwaitable& operator=(CPUAwaitable&&     ) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        [[nodiscard]]
		RkBool Completed() const noexcept;

        #pragma endregion
};

template <typename TReturnType>
RkSize CPUAwaitable<TReturnType>::GetReferenceCount() const noexcept
{
    return m_references.load(std::memory_order_acquire);
}

template <typename TReturnType>
RkVoid CPUAwaitable<TReturnType>::SignalCompletion() noexcept
{
    CPUContinuation::Node* selection {std::addressof(m_continuation_hook)};
    CPUContinuation*       value;

    while(true)
    {
        do
        {
            value = nullptr; // Checking if we have awaiters to signal
            if (selection->compare_exchange_strong(value, CPUContinuation::consumed, std::memory_order_release, std::memory_order_acquire))
                return; 

        // And waiting for any lock in the process
        } while(value == CPUContinuation::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the awaiter 
        if (selection->compare_exchange_weak(value, CPUContinuation::consumed, std::memory_order_release, std::memory_order_acquire))
        {
            // Fetching the next awaiter before notifying the dependent event in case it gets destroyed as a side effect
            CPUContinuation::Node* next = &value->next;
            value->owner->OnContinuation();

            selection = next;
        }
    }
}

template <typename TReturnType>
RkVoid CPUAwaitable<TReturnType>::Reset() noexcept
{
    m_continuation_hook.store(nullptr, std::memory_order_release);
}

template <typename TReturnType>
RkBool CPUAwaitable<TReturnType>::Completed() const noexcept
{
    return m_continuation_hook.load(std::memory_order_acquire) == CPUContinuation::consumed;
}

inline RkVoid CPUAwaitable<RkVoid>::SignalCompletion() noexcept
{
    CPUContinuation::Node* selection {std::addressof(m_continuation_hook)};
    CPUContinuation*       value;

    while(true)
    {
        do
        {
            value = nullptr; // Checking if we have awaiters to signal
            if (selection->compare_exchange_strong(value, CPUContinuation::consumed, std::memory_order_release, std::memory_order_acquire))
                return; 

        // And waiting for any lock in the process
        } while(value == CPUContinuation::locked);

        // If the value of status is still the same as before our last comparison
        // then we can exchange the pointer for a completion pointer and notify the awaiter 
        if (selection->compare_exchange_weak(value, CPUContinuation::consumed, std::memory_order_release, std::memory_order_acquire))
        {
            // Fetching the next awaiter before notifying the dependent event in case it gets destroyed as a side effect
            CPUContinuation::Node* next = &value->next;
            value->owner->OnContinuation();

            selection = next;
        }
    }
}

inline RkVoid CPUAwaitable<RkVoid>::Reset() noexcept
{
    m_continuation_hook.store(nullptr, std::memory_order_release);
}

inline RkBool CPUAwaitable<RkVoid>::Completed() const noexcept
{
    return m_continuation_hook.load(std::memory_order_acquire) == CPUContinuation::consumed;
}


#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl"

END_RUKEN_NAMESPACE