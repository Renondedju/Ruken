#pragma once

#include <type_traits>
#include <variant>

BEGIN_RUKEN_NAMESPACE

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTaskAwaiter<TQueueHandle, TResult>::AsyncTaskAwaiter(
	AsyncAwaiter&&							 in_async_awaiter,
	AsyncTaskPromise<TQueueHandle, TResult>* in_promise) noexcept:
	AsyncAwaiter {std::forward<AsyncAwaiter>(in_async_awaiter)},
	promise		 {in_promise}
{}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTaskAwaiter<TQueueHandle, TResult>::await_resume() const
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

		return std::get<TResult>(promise->result);
	}
}

END_RUKEN_NAMESPACE