#pragma once

#include "JobSystem/Awaitables/AsyncAwaiter.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief A thing that can be waited for. ex: generic events, synchronization primitives or an IO operation.
 * @note Awaitables do not have to inherit from this class to be awaitable.
 *		 The only requirement is having a co_await operator that returns an Awaiter.
 *
 * Awaiters will attempt to detach themselves upon destruction if the event has not been consumed.
 * If an awaitable can be destroyed before all of it's attached awaiters,
 * you must make sure it is consumed by calling Awaitable::Consume().
 */
struct AsyncAwaitable
{
	#pragma region Lifetime

	AsyncAwaitable()							     = default;
	AsyncAwaitable& operator=(AsyncAwaitable const&) = delete;
	AsyncAwaitable& operator=(AsyncAwaitable&&)      = delete;
	AsyncAwaitable(AsyncAwaitable&&)	             = delete;
	AsyncAwaitable(AsyncAwaitable const&)            = delete;
	~AsyncAwaitable()						         = default;

	#pragma endregion

	/// @return Constructs and returns an awaiter that has yet to be attached.
	AsyncAwaiter operator co_await() const noexcept;

	protected:

		// mutable lets us co_await on a const object.
		mutable AsyncAwaiterList m_awaiter_list {nullptr};
};

END_RUKEN_NAMESPACE
