#pragma once

#include "Core/JobSystem/Awaitables/Primitives/ManualResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

struct ResourceLoadEvent : ManualResetEvent
{
	struct Awaiter : RUKEN_NAMESPACE::Awaiter
	{
		ResourceLoadEvent const& owner;

		/// @brief Throws if Trigger has been called with a non-null pointer.
		void await_resume() const;
	};

	/// @brief Signals the end of a load operation.
	RkVoid Trigger(std::exception_ptr const& in_exception = nullptr) noexcept;

	/// @brief Returns an awaiter that waits for the event to be triggered.
	Awaiter operator co_await() const noexcept;

	private:

		std::exception_ptr m_exception {};
};

END_RUKEN_NAMESPACE