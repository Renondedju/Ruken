#pragma once

#include "JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

template<std::predicate<AsyncAwaiter*> TSignalIf>
RkBool AutomaticResetEvent::SignalIf(TSignalIf&& in_signal_if) const noexcept
{
	AsyncAwaiterList const* selection {&m_awaiter_list};
	AsyncAwaiter*           previous  {nullptr};
	AsyncAwaiter*	       continuation;

	while (true)
	{
		// Waiting for any locks
		while ((continuation = selection->load(std::memory_order_acquire)) == AsyncAwaiter::locked)
			;

		// We can signal the previous awaiter.
		// This is done this way in case it gets destroyed as a side effect
		// and avoids us to read potentially unallocated memory.
		if (previous && in_signal_if(previous))
			previous->signal.Invoke();

		// If the continuation is null we reached the end of the chain
		if (continuation == nullptr)
			return previous != nullptr;

		// Fetching the next awaiter
		selection = &continuation->next;
		previous   = continuation;
	}
}

END_RUKEN_NAMESPACE