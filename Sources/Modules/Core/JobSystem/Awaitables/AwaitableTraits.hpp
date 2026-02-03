#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Core/JobSystem/Awaitables/AsyncAwaiter.hpp"

#include <utility>

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept IsAwaiter = requires (TType& in_awaiter) {
	{ in_awaiter.await_ready() } -> std::convertible_to<RkBool>;

	in_awaiter.await_suspend(std::coroutine_handle());
	in_awaiter.await_resume ();
};

template <typename TType>
concept IsAsyncAwaiter = IsAwaiter<TType> && std::derived_from<TType, AsyncAwaiter>;

template <typename TType>
concept IsSyncAwaiter  = IsAwaiter<TType>;

template <typename TType>
concept IsAwaitable = requires (TType const& in_awaitable) {
	{ in_awaitable.operator co_await() } -> IsAwaiter;
};

template <typename TType>
concept IsAsyncAwaitable = requires (TType const& in_awaitable) {
	{ in_awaitable.operator co_await() } -> IsAsyncAwaiter;
};

template <typename TType>
concept IsAwaitableRange = std::ranges::range<TType> && IsAwaitable<std::ranges::range_value_t<TType>>;

template <typename TType>
concept IsAsyncAwaitableRange = std::ranges::range<TType> && IsAsyncAwaitable<std::ranges::range_value_t<TType>>;

template <IsAwaiter TAwaiter>
struct AwaiterTraits
{
	using AwaitSuspendResult = decltype(std::declval<TAwaiter>().await_suspend(std::coroutine_handle()));
	using AwaitResumeResult  = decltype(std::declval<TAwaiter>().await_resume  ());

	static constexpr RkBool await_result_void = std::is_void_v<AwaitResumeResult>;
};

template <IsAwaitable TAwaitable>
struct AwaitableTraits
{
	using Awaiter	  = decltype(std::declval<TAwaitable>().operator co_await());
	using AwaitResult = AwaiterTraits<Awaiter>::AwaitResumeResult;

	static constexpr RkBool await_result_void = AwaiterTraits<Awaiter>::await_result_void;
};

template <IsAwaitableRange TAwaitableRange>
struct AwaitableRangeTraits
{
	using Awaitable   = std::ranges::range_value_t<TAwaitableRange>;
	using Awaiter	  = AwaitableTraits<Awaitable>::Awaiter;
	using AwaitResult = AwaitableTraits<Awaitable>::AwaitResult;

	static constexpr RkBool await_result_void = AwaitableTraits<Awaitable>::await_result_void;
};

/**
 * @brief await_suspend can return void, bool or std::coroutine_handle<>.
 *		  This method mimics the behavior of the compiler transform.
 *
 * @tparam TAwaiter  Awaiter type.
 * @param in_awaiter Awaiter to suspend.
 * @return True if suspension will be completed asynchronously, false if it already has been completed.
 */
template <IsAwaiter TAwaiter>
RkBool DoAwaitSuspend(TAwaiter& in_awaiter)
{
	using AwaitSuspendResult = AwaiterTraits<TAwaiter>::AwaitSuspendResult;

	if constexpr (std::is_void_v<AwaitSuspendResult>) {
		in_awaiter.await_suspend(std::noop_coroutine());
		return IsAsyncAwaiter<TAwaiter>;
	}

	if constexpr (std::is_same_v<AwaitSuspendResult, RkBool>)
		return in_awaiter.await_suspend(std::noop_coroutine());

	// suspension is a coroutine handle to resume
	if constexpr (std::is_same_v<AwaitSuspendResult, std::coroutine_handle<>>)
	{
		std::coroutine_handle<> const suspension {in_awaiter.await_suspend(std::noop_coroutine())};
		suspension.resume();

		RUKEN_ASSERT(IsSyncAwaiter<TAwaiter> && suspension.done(),
			"Suspension is synchronous and coroutine handle is not done after resuming. "
			"TAwaiter should probably be asynchronous."
		);

		// If the suspension coroutine isn't already done,
		// that means it will be completed asynchronously instead
		return !suspension.done();
	}

	std::unreachable();
}

END_RUKEN_NAMESPACE