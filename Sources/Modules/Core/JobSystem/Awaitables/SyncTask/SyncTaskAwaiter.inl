#pragma once

#include <variant>

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTaskAwaiter<TResult>::SyncTaskAwaiter(SyncTaskAwaiter&& in_move) noexcept
{
	std::swap(promise, in_move.promise);
}

template<typename TResult>
SyncTaskAwaiter<TResult>& SyncTaskAwaiter<TResult>::operator=(SyncTaskAwaiter&& in_move) noexcept
{
	std::swap(promise, in_move.promise);

	return *this;
}

template<typename TResult>
RkBool SyncTaskAwaiter<TResult>::await_ready() const noexcept
{
	return false;
}

template<typename TResult>
SyncTaskAwaiter<TResult>::SyncTaskAwaiter(SyncTaskPromise<TResult>* in_promise) noexcept:
	promise {in_promise}
{}

template<typename TResult>
std::coroutine_handle<> SyncTaskAwaiter<TResult>::await_suspend(std::coroutine_handle<> in_continuation) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	promise->continuation = in_continuation;

	// Symmetric transfer. Sync task is ran in place.
	return std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(*promise);
}

template<typename TResult>
auto SyncTaskAwaiter<TResult>::await_resume() const
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if constexpr (std::is_void_v<TResult>)
	{
		if (*promise->result_ptr)
			std::rethrow_exception(*promise->result_ptr);
	}
	else
	{
		if (std::holds_alternative<std::exception_ptr>(*promise->result_ptr))
			std::rethrow_exception(std::get<std::exception_ptr>(*promise->result_ptr));

		return std::move(std::get<TResult>(*promise->result_ptr));
	}
}

END_RUKEN_NAMESPACE