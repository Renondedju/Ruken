#pragma once

#include "Core/JobSystem/Awaitables/Primitives/CountDownLatch.hpp"
#include "Core/JobSystem/Awaitables/SyncTask/SyncTask.hpp"
#include "Core/JobSystem/Awaitables/AwaitableTraits.hpp"

#include <utility>
#include <ranges>

BEGIN_RUKEN_NAMESPACE

/**
 * Waits simultaneously for all the passed awaitables.
 *
 * @tparam TRange		 A sized range type.
 * @param  in_awaitables Awaitables to wait for.
 * @return A vector containing the result of all the waits.
 */
template <IsAsyncAwaitableRange TRange>
requires (!AwaitableRangeTraits<TRange>::await_result_void)
auto WhenAll(TRange const& in_awaitables) noexcept ->
	SyncTask<std::vector<typename AwaitableRangeTraits<TRange>::AwaitResult>>
{
	using TAwaiter   = AwaitableRangeTraits<TRange>::Awaiter;
	using TResult	 = AwaitableRangeTraits<TRange>::AwaitResult;

	CountDownLatch        latch    {std::ranges::size(in_awaitables)};
	std::vector<TAwaiter> awaiters {std::ranges::size(in_awaitables)};
	std::vector<TResult>  results  {std::ranges::size(in_awaitables)};

	// co_await compiler transform
	for (auto const& [awaitable, awaiter] : std::views::zip(in_awaitables, awaiters))
	{
		awaiter		   = awaitable.operator co_await();
		awaiter.signal = SignalReceiver(latch);

		// Trying to suspend & checking if the wait is already over
		if (awaiter.await_ready() || !awaiter.await_suspend(std::coroutine_handle()))
			latch.Signal();
	}

	// Waiting for all tasks to be done
	co_await latch;

	// Gathering results
	for (auto const& [awaiter, result] : std::views::zip(awaiters, results))
		result = std::forward<TResult>(awaiter.await_resume());

	co_return results;
}

/**
 * Waits simultaneously for all the passed awaitables.
 *
 * @tparam TRange		 A sized range type.
 * @param  in_awaitables Awaitables to wait for.
 */
template <IsAsyncAwaitableRange TRange>
requires AwaitableRangeTraits<TRange>::await_result_void
auto WhenAll(TRange const& in_awaitables) noexcept ->
	SyncTask<>
{
	using TAwaiter = AwaitableRangeTraits<TRange>::Awaiter;

	CountDownLatch		  latch    {std::ranges::size(in_awaitables)};
	std::vector<TAwaiter> awaiters {std::ranges::size(in_awaitables)};

	// co_await compiler transform
	for (auto const& [awaitable, awaiter] : std::views::zip(in_awaitables, awaiters))
	{
		awaiter		   = awaitable.operator co_await();
		awaiter.signal = SignalReceiver(latch);

		// Trying to suspend & checking if the wait is already over
		if (awaiter.await_ready() || !awaiter.await_suspend(std::coroutine_handle()))
			latch.Signal();
	}

	// Waiting for all tasks to be done
	co_await latch;

	// Finally: even if we don't gather results, we still need
	// to resume the awaiters to let them know we are done waiting for them.
	// This can be used to propagate exceptions for example.
	for (auto const& awaiter : awaiters)
		awaiter.await_resume();
}

/**
 * Waits simultaneously for all the passed awaitables.
 *
 * @tparam TAwaitables   Awaitable types to wait for.
 * @param  in_awaitables Awaitables to wait for.
 * @return A tuple containing the result of all the waits.
 */
template <IsAsyncAwaitable... TAwaitables>
requires (!AwaitableTraits<TAwaitables>::await_result_void && ...)
auto WhenAll(TAwaitables const&... in_awaitables) ->
	SyncTask<     // Runs on the same queue as the caller
		std::tuple< // And returns a tuple of all the result types of the passed awaitables
			typename AwaitableTraits<TAwaitables>::AwaitResult...
		>
	>
{
	CountDownLatch												  latch    {sizeof...(TAwaitables)};
	std::tuple<typename AwaitableTraits<TAwaitables>::Awaiter...> awaiters {in_awaitables.operator co_await()...};

	// co_await compiler transform
	[&]<auto... Is>(std::index_sequence<Is...>)
	{
		([&](auto& in_awaiter) // foreach awaiter
		{
			in_awaiter.signal = SignalReceiver(latch);

			// Trying to suspend
			if (!in_awaiter.await_ready  () &&
				 in_awaiter.await_suspend(std::coroutine_handle()))
				return;

			latch.Signal();
		}(std::get<Is>(awaiters)), ...);
	}(std::index_sequence_for<TAwaitables...>{});

	co_await latch;

	// Gathering results
	co_return std::apply([&] <typename... TAwaiter> (TAwaiter&... in_awaiters) {
		return std::tuple(std::forward<decltype(in_awaiters.await_resume())>(in_awaiters.await_resume())...);
	}, awaiters);
}


/**
 * Waits simultaneously for all the passed awaitables.
 *
 * @tparam TAwaitables   Awaitable types to wait for.
 * @param  in_awaitables Awaitables to wait for.
 */
template <IsAsyncAwaitable... TAwaitables>
requires (AwaitableTraits<TAwaitables>::await_result_void || ...)
auto WhenAll(TAwaitables const&... in_awaitables) ->
	SyncTask<>
{
	CountDownLatch												  latch    {sizeof...(TAwaitables)};
	std::tuple<typename AwaitableTraits<TAwaitables>::Awaiter...> awaiters {in_awaitables.operator co_await()...};

	// co_await compiler transform
	[&]<auto... Is>(std::index_sequence<Is...>)
	{
		([&](auto& in_awaiter) // foreach awaiter
		{
			in_awaiter.signal = SignalReceiver(latch);

			// Trying to suspend
			if (!in_awaiter.await_ready  () &&
				 in_awaiter.await_suspend(std::coroutine_handle()))
				return;

			latch.Signal();
		}(std::get<Is>(awaiters)), ...);
	}(std::index_sequence_for<TAwaitables...>{});

	co_await latch;

	// Gathering results
	std::apply([&] <typename... TAwaiter> (TAwaiter&... in_awaiters) {
		(in_awaiters.await_resume(), ...);
	}, awaiters);
}

END_RUKEN_NAMESPACE