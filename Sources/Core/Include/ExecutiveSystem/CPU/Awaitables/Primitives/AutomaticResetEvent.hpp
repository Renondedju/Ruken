#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

struct AutomaticResetEvent: CPUAwaitable
{
	protected:

		/// @brief Signals a completion to all the attached awaiters.
		RkBool Signal() const noexcept;

		/**
		 * @brief Signals a completion to a selected set of awaiters.
		 * @param in_signal_if Predicate indicating if the passed awaiter should be signaled.
		 * @returns True if the method signaled any awaiters.
		 */
		template <std::predicate<CPUAwaiter*> TSignalIf>
		RkBool SignalIf(TSignalIf&& in_signal_if) const noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/Primitives/AutomaticResetEvent.inl"
