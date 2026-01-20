#pragma once

#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"

#include <ranges>
#include <concepts>

BEGIN_RUKEN_NAMESPACE

template <std::ranges::range TRange, std::invocable<std::ranges::range_value_t<TRange>&> TFunction>
using ParallelForEachAwaitable = std::invoke_result_t<TFunction, std::ranges::range_value_t<TRange>&>;

/**
 * Invokes in_function for each value in in_value_container and waits for all the returned awaitables.
 *
 * @tparam TRange             Value container type. Must be a sized range.
 * @tparam TFunction          Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param  in_value_container Value container.
 * @param  in_function		  Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction>
	requires AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::await_result_void
SyncTask<> ParallelForeach(TRange const& in_value_container, TFunction&& in_function) noexcept
{
	// Constructing awaitables
	std::vector<ParallelForEachAwaitable<TRange, TFunction>> awaitables {std::ranges::size(in_value_container)};
	for (auto const& [value, awaitable] : std::views::zip(in_value_container, awaitables))
		awaitable = in_function(value);

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

/**
 * Invokes in_function for each value in in_value_container and waits for all the returned awaitables.
 *
 * @tparam TRange             Value container type. Must be a sized range.
 * @tparam TFunction          Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param  in_value_container Value container.
 * @param  in_function		  Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction>
	requires AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::await_result_void
SyncTask<> ParallelForeach(TRange& in_value_container, TFunction&& in_function) noexcept
{
	// Constructing awaitables
	std::vector<ParallelForEachAwaitable<TRange, TFunction>> awaitables {std::ranges::size(in_value_container)};
	for (auto [value, awaitable] : std::views::zip(in_value_container, awaitables))
		awaitable = in_function(value);

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

/**
 * Invokes in_function for each value in in_value_container and waits for all the returned awaitables.
 *
 * @tparam TRange             Value container type. Must be a sized range.
 * @tparam TFunction          Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param  in_value_container Value container.
 * @param  in_function		  Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction>
	requires (!AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::await_result_void)
auto ParallelForeach(TRange const& in_value_container, TFunction&& in_function) noexcept ->
	SyncTask<std::vector<
		typename AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::AwaitResult
	>>
{
	// Constructing awaitables
	std::vector<ParallelForEachAwaitable<TRange, TFunction>> awaitables {std::ranges::size(in_value_container)};
	for (auto const& [value, awaitable] : std::views::zip(in_value_container, awaitables))
		awaitable = std::forward<ParallelForEachAwaitable<TRange, TFunction>>(in_function(value));

	// And waiting for all of them to complete
	co_return co_await WhenAll(awaitables);
}

/**
 * Invokes in_function for each value in in_value_container and waits for all the returned awaitables.
 *
 * @tparam TRange             Value container type. Must be a sized range.
 * @tparam TFunction          Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param  in_value_container Value container.
 * @param  in_function		  Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction>
	requires (!AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::await_result_void)
auto ParallelForeach(TRange& in_value_container, TFunction&& in_function) noexcept ->
	SyncTask<std::vector<
		typename AwaitableTraits<ParallelForEachAwaitable<TRange, TFunction>>::AwaitResult
	>>
{
	// Constructing awaitables
	std::vector<ParallelForEachAwaitable<TRange, TFunction>> awaitables {std::ranges::size(in_value_container)};
	for (auto [value, awaitable] : std::views::zip(in_value_container, awaitables))
		awaitable = std::forward<ParallelForEachAwaitable<TRange, TFunction>>(in_function(value));

	// And waiting for all of them to complete
	co_return co_await WhenAll(awaitables);
}

END_RUKEN_NAMESPACE