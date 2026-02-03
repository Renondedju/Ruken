#pragma once

#include <variant>

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTaskAwaiter<TResult>::SyncTaskAwaiter(
	AsyncAwaiter       const& in_async_awaiter,
	SyncTaskPromise<TResult>* in_promise) noexcept:

	AsyncAwaiter {in_async_awaiter},
	promise		 {in_promise}
{}

template<typename TResult>
auto SyncTaskAwaiter<TResult>::await_resume()
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if constexpr (std::is_void_v<TResult>)
	{
		if (result)
			std::rethrow_exception(result);
	}
	else
	{
		if (std::holds_alternative<std::exception_ptr>(result))
			std::rethrow_exception(std::get<std::exception_ptr>(result));

		return std::move(std::get<TResult>(result));
	}
}

END_RUKEN_NAMESPACE