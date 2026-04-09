#pragma once

#include "Core/JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/Maths/Utility.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Invokes and waits for tasks to run in_function for each value in in_value_container by chunks of in_chunk_size.
 *
 * @tparam TRange             Value container type. Must be a sized range.
 * @tparam TFunction          Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param  in_value_container Value container.
 * @param  in_chunk_size      Amount of entries each task will handle.
 * @param  in_function		  Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction>
	requires std::invocable<TFunction, std::ranges::range_value_t<TRange>&>
SyncTask<> ParallelForeachChunk(TRange const& in_value_container, RkSize const in_chunk_size, TFunction&& in_function) noexcept
{
	if (std::ranges::size(in_value_container) == 0)
		co_return;

	auto process_chunk = [&](RkSize const in_start, RkSize const in_count) -> DynamicTask<RkVoid>
	{
		for (RkSize i = 0; i < in_count; ++i)
			in_function(in_value_container[in_start + i]);

		co_return;
	};

	// Constructing awaitables
	RkSize const					 items      {std::ranges::size(in_value_container)};
	RkSize const					 iterations {static_cast<RkSize>(Ceil(items / static_cast<float>(in_chunk_size)))};
	std::vector<DynamicTask<RkVoid>> awaitables {iterations};
	for (RkSize i = 0; i < iterations; ++i)
		awaitables[i] = process_chunk(i * in_chunk_size, std::min(in_chunk_size, items - i * in_chunk_size));

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

END_RUKEN_NAMESPACE