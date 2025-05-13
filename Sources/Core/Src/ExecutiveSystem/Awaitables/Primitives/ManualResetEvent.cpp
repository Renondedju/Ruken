#include "ExecutiveSystem/Awaitables/Primitives/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

RkVoid ManualResetEvent::Reset() const noexcept
{
	m_awaiter_list.store(nullptr, std::memory_order_release);
}

RkBool ManualResetEvent::Consumed() const noexcept
{
	return m_awaiter_list.load(std::memory_order_acquire) == Awaiter::consumed;
}

RkBool ManualResetEvent::SignalConsume() const noexcept
{
	AwaiterList*   selection {&m_awaiter_list};
	Awaiter const* previous  {nullptr};
	Awaiter*   	   continuation;

	if (selection->load(std::memory_order_acquire) == Awaiter::consumed)
		return false;

	while(true)
	{
		do
		{
			// Checking for continuations to consume
			if (selection->compare_exchange_strong(continuation = nullptr, Awaiter::consumed, std::memory_order_acq_rel))
			{
				// If there is not we need to notify the
				// last awaiter before returning.
				if (previous)
					previous->signal.Invoke();

				return previous != nullptr;
			}

			// If there are, waiting for any lock in the process
		} while(continuation == Awaiter::locked);

		// If the value of status is still the same as before our last comparison
		// then we can exchange the pointer for a completion pointer and notify the previous awaiter
		if (selection->compare_exchange_weak(continuation, Awaiter::consumed, std::memory_order_acq_rel))
		{
			// This is done this way in case it gets destroyed as a side effect
			// and avoids us to read potentially unallocated memory in the code above.
			if (previous)
				previous->signal.Invoke();

			// Fetching the next awaiter
			selection = &continuation->next;
			previous   = continuation;
		}
	}
}
