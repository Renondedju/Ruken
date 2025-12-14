#pragma once

#include "JobSystem/Awaitables/CoroutineTracingUtils.hpp"
#include "JobSystem/Awaitables/AwaitableTraits.hpp"
#include "JobSystem/Queues/JobQueue.hpp"

#include <variant>

BEGIN_RUKEN_NAMESPACE
#define RUKEN_CURRENT_SOURCE_LOCATION [[maybe_unused]] std::source_location in_source_location = std::source_location::current()

template <typename TResult>
struct SyncTask;

/**
 * @brief A synchronous or lazy task.
 * This is mainly used to implement complex synchronisation algorythm and is overall
 * more performant at the cost of some flexibility.
 *
 * Differences with async tasks include:
 *	- Lazy execution: Execution is only started upon await, and only one await can be done per task.
 *	- Always runs in the same queue as the caller.
 *	- Stackfull coroutine.
 *	- Unique handle that moves results instead of copying when available.
 */
template <typename TResult>
struct SyncTaskPromiseBase: CoroutineTracingUtils
{
	JobQueue* queue {nullptr};

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
template <typename TResult>
struct SyncTaskPromise: SyncTaskPromiseBase<TResult>
{
	std::variant<TResult, std::exception_ptr> result;

	// Coroutine exit
	void unhandled_exception()				   noexcept;
	void return_value(TResult&&	     in_value) noexcept;
	void return_value(TResult const& in_value) noexcept;
};

// Result-less specialization
template <>
struct SyncTaskPromise<RkVoid>: SyncTaskPromiseBase<RkVoid>
{
	std::exception_ptr exception;

	// Coroutine exit
	void unhandled_exception() noexcept;
	void return_void        () noexcept;
};

#undef RUKEN_CURRENT_SOURCE_LOCATION

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTaskPromise.inl"
