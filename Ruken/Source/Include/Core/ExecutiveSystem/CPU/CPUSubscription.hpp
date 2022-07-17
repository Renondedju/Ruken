#pragma once

#include <atomic>
#include <coroutine>

#include "Types/FundamentalTypes.hpp"

#include "Core/ExecutiveSystem/Subscription.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central processing unit task subscription base.
 * This class is in charge for the propagation of an event to every awaiting task.
 *
 * This class is basically a thread safe linked list node where AsynchronousEvents
 * contains the head of the list and subscriptions are the actual node containers.
 * Since this node has been designed for co-routines in mind, it can hold some special
 * values listed bellow:
 *
 *  - locked: The next subscription (node) of the list is currently being modified,
 *  and any iteration of the list must be stopped until the pointer is restored.
 *  - completed: The event (list) that this subscription (node) was part of has been completed.
 *
 * Any other values simply acts as a classic linked list node pointer.
 */
struct CPUSubscription: Subscription<CentralProcessingUnit>
{
    using Node = std::atomic<CPUSubscription*>;

    static constexpr CPUSubscription* locked    {reinterpret_cast<CPUSubscription*>(0x1)};
    static constexpr CPUSubscription* completed {reinterpret_cast<CPUSubscription*>(0x2)};

    #pragma region Members

    Node  next {nullptr}; ///< Next subscription in the list
    Node& head;          ///< Reference to the head of the list

    #pragma endregion

    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_head Head of the awaited event
     */
    CPUSubscription(Node& in_head) noexcept;

    CPUSubscription(CPUSubscription const&) = delete;
    CPUSubscription(CPUSubscription&&)      = delete;
    virtual ~CPUSubscription() noexcept; 

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the awaited event has been completed
     */
    virtual RkVoid OnCompletion() noexcept = 0;

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    RkBool await_ready() const noexcept;

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    RkBool await_suspend(std::coroutine_handle<>) noexcept;

    static constexpr RkVoid await_resume() noexcept {}

    #pragma endregion

    #pragma region Operators

    CPUSubscription& operator=(CPUSubscription const&) = delete;
    CPUSubscription& operator=(CPUSubscription&&)      = delete;

    #pragma endregion
};

END_RUKEN_NAMESPACE