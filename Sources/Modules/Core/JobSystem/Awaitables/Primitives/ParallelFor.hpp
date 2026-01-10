#pragma once

#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TIterator, std::invocable<TIterator> TFunction>
using ParallelForAwaitable = std::invoke_result_t<TFunction, TIterator>;

/**
 * Invokes in_function for each iterator value and waits for all the returned awaitables.
 *
 * @tparam TIterator   Iterator type.
 * @tparam TFunction   Method to invoke for each iterator value. Must return an awaitable.
 * @param  in_start	   Start iterator.
 * @param  in_end	   End Iterator.
 * @param  in_function Method to invoke for each iterator value. Must return an awaitable.
 * @return An awaitable.
 */
template <typename TIterator, std::invocable<TIterator> TFunction>
	requires AwaitableTraits<ParallelForAwaitable<TIterator, TFunction>>::await_result_void
SyncTask<> ParallelFor(TIterator in_start, TIterator in_end, TFunction&& in_function) noexcept
{
	// Constructing awaitables
	std::vector<ParallelForAwaitable<TIterator, TFunction>> awaitables {std::distance(in_start, in_end)};
	auto awaitable_it {awaitables.begin()};
	for (TIterator iterator {in_start}; iterator != in_end; ++iterator)
		*(awaitable_it++) = in_function(iterator);

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

/**
 * Invokes in_function for each iterator value and waits for all the returned awaitables.
 *
 * @tparam TIterator   Iterator type.
 * @tparam TFunction   Method to invoke for each iterator value. Must return an awaitable.
 * @param  in_start	   Start iterator.
 * @param  in_end	   End Iterator.
 * @param  in_function Method to invoke for each iterator value. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <typename TIterator, std::invocable<TIterator> TFunction>
	requires (!AwaitableTraits<ParallelForAwaitable<TIterator, TFunction>>::await_result_void)
auto ParallelFor(TIterator in_start, TIterator in_end, TFunction&& in_function) noexcept ->
	SyncTask<std::vector<
		typename AwaitableTraits<ParallelForAwaitable<TIterator, TFunction>>::AwaitResult
	>>
{
	// Constructing awaitables
	std::vector<ParallelForAwaitable<TIterator, TFunction>> awaitables {std::distance(in_start, in_end)};
	auto awaitable_it {awaitables.begin()};
	for (TIterator iterator {in_start}; iterator != in_end; ++iterator)
		*(awaitable_it++) = in_function(iterator);

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

END_RUKEN_NAMESPACE