#pragma once

#include "JobSystem/Awaitables/Awaiter.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief A thing that can be waited for. ex: generic events, synchronization primitives or an IO operation.
 *
 * Continuations will attempt to detach themselves upon destruction if the event has not been consumed.
 * If an awaitable can be destroyed before all of it's attached continuations,
 * you must make sure it is consumed by calling CPUAwaitable::Consume().
 */
struct Awaitable
{
	/// @return Constructs and returns an awaiter that has yet to be attached.
	Awaiter operator co_await() const noexcept;

	protected:

		// mutable lets us co_await on a const object.
		mutable AwaiterList m_awaiter_list {nullptr};
};

END_RUKEN_NAMESPACE
