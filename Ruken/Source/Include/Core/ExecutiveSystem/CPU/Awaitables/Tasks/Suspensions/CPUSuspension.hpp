#pragma once

#include <atomic>
#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Suspension.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUAwaitableCallback.hpp"

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

    CPUAwaitableCallback& completion_callback;

    #pragma region

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept
    {
        // Since we are not attached yet, we can use the head directly
        return head.load(std::memory_order_acquire) == consumed;
    }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept
    {
        CPUSuspension* head_value {head.load(std::memory_order_acquire)};

        do
        {
            while (head_value == locked) // If the current value is locked we need to wait
                head_value = head.load(std::memory_order_acquire);

            // Checking if the event signaled a completion in the meanwhile
            if (head_value == consumed)
                return false; // Operation failed

            // Update linked list to point at the current head
            next.store(head_value, std::memory_order_release);

        // Finally, if the head we originally fetched is still the actual head
        // (it could have been signaled, locked or swapped while we were testing stuff)
        // swapping the old list head with this awaiter as the new list head.
        } while (!head.compare_exchange_weak(head_value, this,
            std::memory_order_release,
            std::memory_order_acquire));

        // Operation succeeded
        return true;
    }

    static constexpr void await_resume() noexcept {}

    #pragma region
};


template <typename TReturnType = RkVoid>
struct CPUReturningSuspension: CPUSuspension
{
    using Node = std::atomic<CPUSuspension*>;

    TReturnType& return_value;

    #pragma region Initialization/Copy

    CPUReturningSuspension(Node& in_head, CPUAwaitableCallback& in_completion_callback, TReturnType& in_return_value) noexcept:
        CPUSuspension {.head = in_head, .completion_callback = in_completion_callback},
        return_value  {in_return_value}
    {}

    virtual ~CPUReturningSuspension() noexcept
    {
        // If the awaiter hasn't been completed in due time,
        // we need to detach it from the awaited event to cancel
        // our wait without crashing later down the line
        if (next.load(std::memory_order_acquire) == consumed)
            return;

        // Attempting to detach from the awaited event
        Node*          selection {std::addressof(head)};
        CPUSuspension* expected  {this};

        // If this awaiter is the one we were looking for, then we lock it to ensure nobody swaps our `next` pointer
        while(!selection->compare_exchange_strong(expected, locked, std::memory_order_acq_rel, std::memory_order_acquire))
        {
            // Otherwise we need to check if the event hasn't been signaled in the meantime
            if (expected == consumed)
                return;

            // And if the selection isn't currently locked, then we can finally test the next awaiter in the list
            // otherwise we'll just retry until the lock has been released
            if (expected != locked)
                selection = &expected->next;

            expected = this;
        }

        // Lock acquired, we can now safely read the next pointer
        // and swap our lock with that, effectively releasing our lock
        selection->store(next.load(std::memory_order_acquire), std::memory_order_release);
    }

    CPUReturningSuspension(CPUReturningSuspension const&) = delete;
    CPUReturningSuspension(CPUReturningSuspension&&)      = delete;

    CPUReturningSuspension& operator=(CPUReturningSuspension const&) = delete;
    CPUReturningSuspension& operator=(CPUReturningSuspension&&)      = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the suspension is resumed.
     * \returns The result of the suspension
     */
    TReturnType const& await_resume() const noexcept
    {
        return return_value;
    }

    #pragma endregion
};

template <>
struct CPUReturningSuspension<RkVoid>: CPUSuspension
{};

END_RUKEN_NAMESPACE
