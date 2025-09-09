#include "Resources/ResourceLoadEvent.hpp"

USING_RUKEN_NAMESPACE

void ResourceLoadEvent::Awaiter::await_resume() const
{
	if (owner.m_exception)
		std::rethrow_exception(owner.m_exception);
}

// ReSharper disable once CppMemberFunctionMayBeConst
//
// Trigger is non const on purpose, this makes it so that having a
// const references to the object allows to wait for it but not trigger it.
RkVoid ResourceLoadEvent::Trigger(std::exception_ptr const& in_exception) noexcept
{
	m_exception = in_exception;

	// Must be called after assignments for thread safety
	SignalConsume();
}

ResourceLoadEvent::Awaiter ResourceLoadEvent::operator co_await() const noexcept
{
	return Awaiter {
		ManualResetEvent::operator co_await(),
		*this
	};
}
