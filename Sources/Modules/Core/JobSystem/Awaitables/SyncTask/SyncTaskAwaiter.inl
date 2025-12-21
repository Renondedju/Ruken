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
}

template<typename TResult>
RkBool SyncTaskAwaiter<TResult>::await_ready() const noexcept
{
	// Lazy tasks are ran when awaited and thus will never be ready before.
	return false;
}

template<typename TResult>
SyncTaskAwaiter<TResult>::SyncTaskAwaiter(SyncTaskPromise<TResult>* in_promise) noexcept:
	promise {in_promise}
{}

template<typename TResult>
SyncTaskAwaiter<TResult>::~SyncTaskAwaiter() noexcept
{
	// The wait is done, result has been moved out of the promise.
	if (promise)
		std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(*promise).destroy();
}

template<typename TResult>
std::coroutine_handle<> SyncTaskAwaiter<TResult>::await_suspend(std::coroutine_handle<> in_continuation) noexcept
{
	promise->continuation = in_continuation;

	// Symmetric transfer. Lazy task is ran in place.
	return std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(*promise);
}

template<typename TResult>
auto SyncTaskAwaiter<TResult>::await_resume() const
{
	if constexpr (std::is_void_v<TResult>)
	{
		if (promise->exception)
			std::rethrow_exception(promise->exception);
	}
	else
	{
		if (std::holds_alternative<std::exception_ptr>(promise->result))
			std::rethrow_exception(std::get<std::exception_ptr>(promise->result));

		return std::move(std::get<TResult>(promise->result));
	}
}

END_RUKEN_NAMESPACE