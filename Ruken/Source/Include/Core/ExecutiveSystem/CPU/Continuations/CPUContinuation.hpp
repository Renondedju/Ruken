#pragma once

#include <atomic>

#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUAwaiter.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central processing unit task subscription base.
 * This class is in charge for the propagation of an event to every awaiting task.
 *
 * This class is basically a thread safe linked list node in conjunction with CPUAwaiter where awaiters
 * contains the head of the list and subscriptions are the actual node containers.
 * Since this node has been designed for co-routines in mind, it can hold some special
 * values listed bellow:
 *
 *  - locked: The next subscription (node) of the list is currently being modified,
 *  and any iteration of the list must be stopped until the pointer is restored.
 *  - expired: The event (list) that this subscription (node) was part of has expired.
 *
 * Any other values simply acts as a classic linked list node pointer.
 */
struct CPUContinuation
{
    using Node = std::atomic<CPUContinuation*>;

    static constexpr CPUContinuation* locked   {reinterpret_cast<CPUContinuation*>(0x1)};
    static constexpr CPUContinuation* consumed {reinterpret_cast<CPUContinuation*>(0x2)};

    #pragma region Members

    Node* hook {nullptr};  ///< Reference to the head of the list
    Node  next {nullptr}; ///< Next subscription in the list

    CPUAwaiter* owner {};

    #pragma region

    #pragma region Methods

    template <typename TReturnType>
    RkVoid Setup(CPUAwaiter& in_owner, CPUAwaitableHandle<TReturnType> const& in_awaited) noexcept
    {
        hook  = std::addressof(in_awaited.m_instance->m_continuation_hook);
        owner = std::addressof(in_owner);
    }

    /**
     * \brief Checks if the event we want to wait for has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool IsEventCompleted() const noexcept;

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool TryAttach() noexcept;

    /**
     * \brief Detaches the continuation 
     */
    RkBool Detach();

    #pragma region
};

END_RUKEN_NAMESPACE