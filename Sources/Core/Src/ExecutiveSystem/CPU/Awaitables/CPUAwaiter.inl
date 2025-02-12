#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"
#include "Meta/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TSignalValue>
CPUAwaiter<TSignalValue>::CPUAwaiter(CPUAwaiterList<TSignalValue>* in_head) noexcept:
	head {in_head}
{}

template<typename TSignalValue>
CPUAwaiter<TSignalValue>::CPUAwaiter(CPUAwaiter const& in_other) noexcept
{
	head   = in_other.head;
	signal = in_other.signal;
	tag    = in_other.tag;
}

template<typename TSignalValue>
CPUAwaiter<TSignalValue>::CPUAwaiter(CPUAwaiter&& in_other) noexcept
{
	head   = std::move(in_other.head);
	signal = std::move(in_other.signal);
	tag    = std::move(in_other.tag);
}

template<typename TSignalValue>
CPUAwaiter<TSignalValue>& CPUAwaiter<TSignalValue>::operator=(CPUAwaiter const& in_other) noexcept
{
	TryDetach();

	head   = in_other.head;
	signal = in_other.signal;
	tag    = in_other.tag;

	return *this;
}

template<typename TSignalValue>
CPUAwaiter<TSignalValue>& CPUAwaiter<TSignalValue>::operator=(CPUAwaiter&& in_other) noexcept
{
	TryDetach();

	head   = std::move(in_other.head);
	signal = std::move(in_other.signal);
	tag    = std::move(in_other.tag);

	return *this;
}

template<typename TSignalValue>
CPUAwaiter<TSignalValue>::~CPUAwaiter()
{
	TryDetach();
}

template<typename TSignalValue>
RkBool CPUAwaiter<TSignalValue>::TryAttach() noexcept
{
	CPUAwaiter* head_value {head->load(std::memory_order_acquire)};

	do
	{
		while (head_value == locked) // If the current value is locked we need to wait
			head_value = head->load(std::memory_order_acquire);

		// Checking if the event signaled a completion in the meanwhile
		if (head_value == consumed)
		{
			next.store(detached, std::memory_order_release);
			return false;
		}

		// Update linked list to point at the current head
		next.store(head_value, std::memory_order_release);

		// Finally, if the head we originally fetched is still the actual head
		// (it could have been signaled, locked or swapped while we were testing stuff)
		// swapping the old list head with this awaiter as the new list head.
	} while (!head->compare_exchange_weak(head_value, this, std::memory_order_release, std::memory_order_acquire));

	// Operation succeeded
	return true;
}

template<typename TSignalValue>
RkBool CPUAwaiter<TSignalValue>::TryDetach() noexcept
{
	// If the awaiter hasn't been completed in due time,
	// we need to detach it from the awaited event to cancel
	// our wait without crashing later down the line
	CPUAwaiter* next_val {next.load(std::memory_order_acquire)};
	if (next_val == detached || next_val == consumed)
		return false;

	// Attempting to detach from the awaited event by looking for our
	// address though the list of suspensions
	CPUAwaiterList<TSignalValue>* selection {head};
	CPUAwaiter*					  expected  {this};

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
	next.store(detached, std::memory_order_relaxed);

	return true;
}

template<typename TSignalValue>
RkBool CPUAwaiter<TSignalValue>::Consumed() const noexcept
{
	return head && head->load(std::memory_order_acquire) == consumed;
}


END_RUKEN_NAMESPACE

