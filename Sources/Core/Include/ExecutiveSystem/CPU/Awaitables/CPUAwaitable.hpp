#pragma once

#include "ExecutiveSystem/Awaitable.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief This class acts as a base for every CPU awaitable and is meant to be inherited from.
 *
 * Continuations will attempt to detach themselves upon destruction if the event has not been consumed.
 * If an awaitable can be destroyed before all of it's attached continuations,
 * you must make sure it is consumed by calling CPUAwaitable::Consume().
 */
struct CPUAwaitable: Awaitable<CentralProcessingUnit>
{
	/// @return Constructs and returns an awaiter that has yet to be attached.
	CPUAwaiter operator co_await() const noexcept;

	protected:

		// mutable lets us co_await on a const object.
		mutable CPUAwaiterList m_awaiter_list {nullptr};
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl"
