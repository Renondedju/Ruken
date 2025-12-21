#pragma once

#include "Core/JobSystem/Awaitables/Primitives/ManualResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

struct ResourceLoadEvent : ManualResetEvent
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_triggered True if Trigger should be called during construction.
	 */
	explicit ResourceLoadEvent(RkBool in_triggered = false) noexcept;
	ResourceLoadEvent(ResourceLoadEvent&&)	    		   = delete;
	ResourceLoadEvent(ResourceLoadEvent const&) 		   = delete;
	ResourceLoadEvent& operator=(ResourceLoadEvent const&) = delete;
	ResourceLoadEvent& operator=(ResourceLoadEvent&&)      = delete;
	~ResourceLoadEvent() = default;

	#pragma endregion

	struct Awaiter;

	/// @brief Signals the end of a load operation.
	RkVoid Trigger(std::exception_ptr const& in_exception = nullptr) noexcept;

	/// @brief Returns an awaiter that waits for the event to be triggered.
	Awaiter operator co_await() const noexcept;

	private:

		std::exception_ptr m_exception {};
};

struct ResourceLoadEvent::Awaiter: AsyncAwaiter
{
	ResourceLoadEvent const& owner;

	/// @brief Throws if Trigger has been called with a non-null pointer.
	void await_resume() const;
};

END_RUKEN_NAMESPACE