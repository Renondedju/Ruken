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
concept IsAwaitable = requires (TType const& in_awaitable) {
	{ in_awaitable.operator co_await() } -> IsAwaiter;
};

template <typename TType>
concept IsAsyncAwaitable = requires (TType const& in_awaitable) {
	{ in_awaitable.operator co_await() } -> IsAsyncAwaiter;
};

template <typename TType>
concept IsAsyncAwaitableRange = std::ranges::range<TType> && IsAsyncAwaitable<std::ranges::range_value_t<TType>>;

template <IsAsyncAwaitable TAwaitable>
struct AwaitableTraits
{
	using Awaiter	  = decltype(std::declval<TAwaitable>().operator co_await());
	using AwaitResult = decltype(std::declval<Awaiter>   ().await_resume     ());

	static constexpr RkBool await_result_void = std::is_void_v<AwaitResult>;
};

template <IsAsyncAwaitableRange TAwaitableRange>
struct AwaitableRangeTraits
{
	using Awaitable   = std::ranges::range_value_t<TAwaitableRange>;
	using Awaiter	  = AwaitableTraits<Awaitable>::Awaiter;
	using AwaitResult = AwaitableTraits<Awaitable>::AwaitResult;

	static constexpr RkBool await_result_void = AwaitableTraits<Awaitable>::await_result_void;
};


END_RUKEN_NAMESPACE