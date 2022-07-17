#include "Core/ExecutiveSystem/CPU/CPUSubscription.hpp"

USING_RUKEN_NAMESPACE

CPUSubscription::CPUSubscription(Node& in_head) noexcept:
    head {in_head}
{}

CPUSubscription::~CPUSubscription() noexcept
{
    // If the awaiter hasn't been completed in due time,
    // we need to detach it from the awaited event to cancel
    // our wait without crashing later down the line
    if (next.load(std::memory_order_acquire) == completed)
        return;

    // Attempting to detach from the awaited event
    Node*                selection {std::addressof(head)};
    CPUSubscription* expected  {this};

    // If this awaiter is the one we were looking for, then we lock it to ensure nobody swaps our `next` pointer
    while(!selection->compare_exchange_strong(expected, locked, std::memory_order_acq_rel, std::memory_order_acquire))
    {
        // Otherwise we need to check if the event hasn't been signaled in the meantime
        if (expected == completed)
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

RkBool CPUSubscription::await_ready() const noexcept
{
    // Since we are not attached yet, we can use the head directly
    return head.load(std::memory_order_acquire) == completed;
}

RkBool CPUSubscription::await_suspend(std::coroutine_handle<>) noexcept
{
    CPUSubscription* temp_head;

    do
    {
        do { // If the current value is locked we need to wait
            temp_head = head.load(std::memory_order_acquire);
        } while (temp_head == locked);

        // Checking if the event signaled a completion in the meanwhile
        if (temp_head == completed)
            return false; // Operation failed

        // Update linked list to point at the current head
        next.store(temp_head, std::memory_order_release);

    // Finally, if the head we originally fetched is still the actual head
    // (it could have been signaled, locked or swapped while we were testing stuff)
    // swapping the old list head with this awaiter as the new list head.
    } while (!head.compare_exchange_weak(temp_head, this,
        std::memory_order_release,
        std::memory_order_acquire));

    // Operation succeeded
    return true;
}
