#pragma once

#include "Core/JobSystem/Awaitables/Primitives/CountDownLatch.hpp"
#include "Core/JobSystem/Awaitables/Tasks/DynamicTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"

#include <ranges>

BEGIN_RUKEN_NAMESPACE

/**
 * Invokes in_function for each value in in_value_container and waits for all the returned awaitables.
 * TODO: Overload to gather and return results.
 *
 * @tparam TRange Value container type. Must be a sized range.
 * @tparam TFunction Method to invoke for each value in in_value_container. Must return an awaitable.
 * @param in_value_container Value container.
 * @param in_function Method to invoke for each value in in_value_container. Must return an awaitable.
 * @return An awaitable that returns a vector of the results.
 */
template <std::ranges::sized_range TRange, typename TFunction> requires
	CAwaitable<std::invoke_result_t<TFunction, std::ranges::range_value_t<TRange>>> &&
			    std::is_invocable_v<TFunction, std::ranges::range_value_t<TRange>>

DynamicTask<RkVoid> ParallelForeach(TRange const& in_value_container, TFunction&& in_function) noexcept
{
	using TFunctionResult = std::invoke_result_t<TFunction, std::ranges::range_value_t<TRange>>;
	using TAwaiter        = AwaiterType<TFunctionResult>;

	CountDownLatch               latch      {std::ranges::size(in_value_container)};
	std::vector<TFunctionResult> awaitables {std::ranges::size(in_value_container)};
	std::vector<TAwaiter>        awaiters   {std::ranges::size(in_value_container)};

	// co_await compiler transform
	for (auto const& [value, awaitable, awaiter] : std::views::zip(in_value_container, awaitables, awaiters))
	{
		awaitable      = in_function(value);
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
	for (auto& awaiter : awaiters)
		awaiter.await_resume();

	co_return;
}

END_RUKEN_NAMESPACE