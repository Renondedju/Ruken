#pragma once

#include <atomic>
#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Suspension.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

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
struct CPUSuspension: Suspension<CentralProcessingUnit>
{
    using Node = std::atomic<CPUSuspension*>;

    static constexpr CPUSuspension* locked   {reinterpret_cast<CPUSuspension*>(0x1)};
    static constexpr CPUSuspension* consumed {reinterpret_cast<CPUSuspension*>(0x2)};

    #pragma region Members

    Node& head;            ///< Reference to the head of the list
    Node  next {nullptr}; ///< Next subscription in the list

    #pragma endregion

    #pragma region Initialization/Copy

    CPUSuspension(Node& in_head) noexcept;
    CPUSuspension(CPUSuspension const&) = delete;
    CPUSuspension(CPUSuspension&&)      = delete;
    virtual ~CPUSuspension() noexcept;

    CPUSuspension& operator=(CPUSuspension const&) = delete;
    CPUSuspension& operator=(CPUSuspension&&)      = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept;

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept;

    /**
     * \brief Called when the awaited event has been completed
     * Do note that this event is only called if a suspension has been completed
     */
    virtual RkVoid OnCompletion() noexcept = 0;

    #pragma endregion
};

END_RUKEN_NAMESPACE