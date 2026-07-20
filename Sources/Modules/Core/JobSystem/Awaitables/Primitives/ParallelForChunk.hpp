#pragma once

#include "Core/JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/Maths/Utility.hpp"
#include "Core/Debug/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Invokes and waits for tasks to run in_function for each index value by chunks of in_chunk_size.
 *
 * @tparam TIndex	     Index type.
 * @tparam TFunction     Method to invoke for each index value. Must return an awaitable.
 * @param  in_start	     Start index.
 * @param  in_end	     End index.
 * @param  in_chunk_size Amount of entries each task will handle.
 * @param  in_function   Method to invoke for each index value. Must return an awaitable.
 * @return An awaitable.
 */
template <typename TIndex, std::invocable<TIndex> TFunction>
	requires std::invocable<TFunction, TIndex>
SyncTask<> ParallelForChunk(
	TIndex 		  in_start,
	TIndex 		  in_end,
	TIndex const  in_chunk_size,
	TFunction&&   in_function) noexcept
{
	// Early out
	RUKEN_ASSERT(in_start <= in_end, "in_start must be less than in_end");
	if (in_start == in_end)
		co_return;

	RkSize const 			   items      {in_end - in_start};
	RkSize const 			   iterations {static_cast<RkSize>(Ceil(items / static_cast<float>(in_chunk_size)))};
	std::vector<DynamicTask<>> awaitables {iterations};

	// Constructing awaitables
	for (RkSize i = 0; i < iterations; ++i)
	{
		awaitables[i] = [&](TIndex const in_start_index, TIndex const in_count) -> DynamicTask<>
		{
			for (TIndex index {0}; index < in_count; ++index)
				in_function(in_start_index + index);

			co_return;
		}(i * in_chunk_size, std::min(in_chunk_size, items - i * in_chunk_size));
	}

	// And waiting for all of them to complete
	co_await WhenAll(awaitables);
}

END_RUKEN_NAMESPACE