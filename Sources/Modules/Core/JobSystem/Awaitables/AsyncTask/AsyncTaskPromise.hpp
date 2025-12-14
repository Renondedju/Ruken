#pragma once

#include "JobSystem/Queues/IsQueueHandle.hpp"
#include "JobSystem/Awaitables/AwaitableTraits.hpp"
#include "JobSystem/Awaitables/CoroutineTracingUtils.hpp"
#include "JobSystem/Awaitables/Primitives/ManualResetEvent.hpp"

#include "Debug/Trace.hpp"

#include <source_location>
#include <variant>

BEGIN_RUKEN_NAMESPACE
#define RUKEN_CURRENT_SOURCE_LOCATION [[maybe_unused]] std::source_location in_source_location = std::source_location::current()

template <IsQueueHandle TQueueHandle, typename TResult>
struct AsyncTask;

/**
 * @brief An asynchronous or greedy task.
 * This is the more flexible of the two. This kind of task should be used when parallelization and latency matters.
 *
 * Differences with sync tasks include:
 *	- Greedy execution: Execution is only started as soon as the coroutine is invoked.
 *	- Runs in a user defined queue.
 *	- Stackless coroutine.
 *	- Reference counted lifetime, can be waited by multiple consumers at once.
 */
template <IsQueueHandle TQueueHandle, typename TResult>
struct AsyncTaskPromiseBase: CoroutineTracingUtils, ManualResetEvent
{
	// Async tasks are reference counted.
	std::atomic<RkSize> references {1ULL};

	/// @returns an awaiter that waits for the task to return or throw an exception.
	auto operator co_await(this auto&) noexcept;

	/// @brief Called when an asynchronous wait is over
	RkVoid Signal(this auto&) noexcept;

	/// @brief Instantiates and returns the task handle.
	auto get_return_object(this auto&) noexcept;

	/// @brief Composes and returns a new awaitable that inherits from TAwaitable to
	///		   inject behavior such as automatic tracing and task resuming.
	template<IsAwaitable TAwaitable>
	auto await_transform(TAwaitable const& in_awaitable, RUKEN_CURRENT_SOURCE_LOCATION) noexcept;

	// Coroutine lifetime
	auto initial_suspend(this auto&, RUKEN_CURRENT_SOURCE_LOCATION) noexcept;
	auto final_suspend  () noexcept;
};

// Result specialization
template<IsQueueHandle TQueueHandle, typename TResult>
struct AsyncTaskPromise: AsyncTaskPromiseBase<TQueueHandle, TResult>
{
	std::variant<TResult, std::exception_ptr> result;

	// Coroutine exit
	void unhandled_exception()					noexcept;
	void return_value(TResult const& in_result) noexcept;
	void return_value(TResult&&	     in_result) noexcept;
};

// Result-less specialization
template <IsQueueHandle TQueueHandle>
struct AsyncTaskPromise<TQueueHandle, RkVoid>: AsyncTaskPromiseBase<TQueueHandle, RkVoid>
{
	std::exception_ptr exception;

	// Coroutine exit
	void unhandled_exception() noexcept;
	void return_void		() noexcept;
};

#undef RUKEN_CURRENT_SOURCE_LOCATION

END_RUKEN_NAMESPACE

#include "AsyncTaskPromise.inl"
