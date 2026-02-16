#pragma once

#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TIterator, std::invocable<TIterator> TFunction>
using ParallelForAwaitable = std::invoke_result_t<TFunction, TIterator>;

/**
 * Invokes in_function for each index value and waits for all the returned awaitables.
 *
 * @tparam TIndex	   Index type.
 * @tparam TFunction   Method to invoke for each index value. Must return an awaitable.
 * @param  in_start	   Start index.
 * @param  in_end	   End index.
 * @param  in_function Method to invoke for each index value. Must return an awaitable.
 * @return An awaitable.
 */
template <typename TIndex, std::invocable<TIndex> TFunction>
	requires AwaitableTraits<ParallelForAwaitable<TIndex, TFunction>>::await_result_void
SyncTask<> ParallelFor(TIndex in_start, TIndex in_end, TFunction&& in_function) noexcept
{
	// Constructing awaitables
	std::vector<ParallelForAwaitable<TIndex, TFunction>> awaitables(in_end - in_start);
	auto awaitable_it {awaitables.begin()};
	for (TIndex index {in_start}; index != in_end; ++index)
		*(awaitable_it++) = in_function(index);

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

/**
 * Invokes in_function for each index value and waits for all the returned awaitables.
 *
 * @tparam TIndex	   Index type.
 * @tparam TFunction   Method to invoke for each index value. Must return an awaitable.
 * @param  in_start	   Start index.
 * @param  in_end	   End index.
 * @param  in_function Method to invoke for each index value. Must return an awaitable.
 * @return An awaitable.
 */
template <typename TIndex, std::invocable<TIndex> TFunction>
	requires (!AwaitableTraits<ParallelForAwaitable<TIndex, TFunction>>::await_result_void)
auto ParallelFor(TIndex in_start, TIndex in_end, TFunction&& in_function) noexcept ->
	SyncTask<std::vector<
		typename AwaitableTraits<ParallelForAwaitable<TIndex, TFunction>>::AwaitResult
	>>
{
	// Constructing awaitables
	std::vector<ParallelForAwaitable<TIndex, TFunction>> awaitables(in_end - in_start);
	auto awaitable_it {awaitables.begin()};
	for (TIndex index {in_start}; index != in_end; ++index)
		*(awaitable_it++) = in_function(index);

	// And waiting for all of them to complete
	co_return co_await WhenAll(awaitables);
}

END_RUKEN_NAMESPACE