#pragma once

#include "JobSystem/Awaitables/AsyncAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

struct ManualResetEvent: AsyncAwaitable
{
	/// @return Returns true if the event has already been completed, false otherwise
	[[nodiscard]]
	RkBool Consumed() const noexcept;

	protected:

		/// @brief Resets the event, making it non consumed and ready to hook new awaiters.
		RkVoid Reset() const noexcept;

		/// @brief Consumes and signals the list of awaiters.
		RkBool SignalConsume() const noexcept;
};

END_RUKEN_NAMESPACE