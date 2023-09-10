#pragma once

#include <atomic>

#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUAwaiter.hpp"

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

    Node* hook {};         ///< Reference to the head of the list
    Node  next {nullptr}; ///< Next subscription in the list

    CPUAwaiter* owner {};

    #pragma region

    #pragma region Lifetime

    CPUContinuation() = default;
    CPUContinuation(CPUContinuation const&) = delete;
    CPUContinuation(CPUContinuation&&     ) = delete;
    ~CPUContinuation()                      = default;

    CPUContinuation& operator=(CPUContinuation const&) = delete;
    CPUContinuation& operator=(CPUContinuation&&     ) = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Checks if the event we want to wait for has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool IsEventCompleted() const noexcept
    {
        // Since we are not attached yet, we can use the head directly
        return hook->load(std::memory_order_acquire) == consumed;
    }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool TryAttach() noexcept
    {
        CPUContinuation* head_value {hook->load(std::memory_order_acquire)};

        do
        {
            while (head_value == locked) // If the current value is locked we need to wait
                head_value = hook->load(std::memory_order_acquire);

            // Checking if the event signaled a completion in the meanwhile
            if (head_value == consumed)
                return false; // Operation failed

            // Update linked list to point at the current head
            next.store(head_value, std::memory_order_release);

        // Finally, if the head we originally fetched is still the actual head
        // (it could have been signaled, locked or swapped while we were testing stuff)
        // swapping the old list head with this awaiter as the new list head.
        } while (!hook->compare_exchange_weak(head_value, this,
            std::memory_order_release,
            std::memory_order_acquire));

        // Operation succeeded
        return true;
    }

    /**
     * \brief Detaches the continuation 
     */
    RkBool Detach()
    {
        // If the awaiter hasn't been completed in due time,
        // we need to detach it from the awaited event to cancel
        // our wait without crashing later down the line
        if (next.load(std::memory_order_acquire) == consumed)
            return false;

        // Attempting to detach from the awaited event by looking for our
        // address though the list of suspensions
        Node*            selection {hook};
        CPUContinuation* expected  {this};

        // If this awaiter is the one we were looking for, then we lock it to ensure nobody swaps our `next` pointer
        while(!selection->compare_exchange_strong(expected, locked, std::memory_order_acq_rel, std::memory_order_acquire))
        {
            // Otherwise we need to check if the event hasn't been signaled in the meantime
            // or if we haven't found ourselves in the list for some reason
            if (expected == consumed ||
                expected == nullptr)
                return false;

            // And if the selection isn't currently locked, then we can finally test the next awaiter in the list
            // otherwise we'll just retry until the lock has been released
            if (expected != locked)
                selection = &expected->next;

            expected = this;
        }

        // Lock acquired, we can now safely read the next pointer
        // and swap our lock with that, effectively releasing our lock
        selection->store(next.load(std::memory_order_acquire), std::memory_order_release);

        return true;
    }

    #pragma region
};

END_RUKEN_NAMESPACE