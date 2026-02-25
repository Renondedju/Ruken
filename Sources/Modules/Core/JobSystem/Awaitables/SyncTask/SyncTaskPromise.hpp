#pragma once

#include "Core/JobSystem/Queues/JobQueue.hpp"
#include "Core/JobSystem/Awaitables/AwaitableTraits.hpp"
#include "Core/JobSystem/Awaitables/CoroutineTracingUtils.hpp"
#include "Core/JobSystem/Awaitables/SyncTask/SyncTaskResult.hpp"
#include "Core/JobSystem/Awaitables/Primitives/ManualResetEvent.hpp"

#include <source_location>

BEGIN_RUKEN_NAMESPACE
#define RUKEN_CURRENT_SOURCE_LOCATION [[maybe_unused]] std::source_location = std::source_location::current()

template <typename TResult>
struct SyncTask;

/**
 * @brief A synchronous or synchronization task.
 * @tparam TResult Return value of the task.
 *
 * This is mainly used to implement various synchronization algorithms as the coroutine is always ran synchronously when instantiated.
 * Execution is scheduled back to the original queue when an asynchronous wait is over.
 * This is useful in scenarios where code execution needs to follow a precise order.
 * Because of this 'inlining' behavior, these tasks are meant to be somewhat lightweight to avoid slowing down asynchronous parallel waits.
 *
 * Differences with async tasks include:
 *	- Execution is started by the caller on instantiation (instead of being scheduled to a queue).
 *	- Can be a stackfull coroutine (there is no reliable way to measure that yet in c++26).
 *	- Unique handle that moves results instead of copying when available. Only one await can be done per task.
 */
template <typename TResult>
struct SyncTaskPromiseBase: CoroutineTracingUtils, ManualResetEvent
{
	JobQueue*			queue   {nullptr};
	std::atomic<RkSize> references {2ULL};

	/// @returns an awaiter that waits for the task to return or throw an exception.
	auto operator co_await(this auto&&) noexcept;

	/// @brief Called when an asynchronous wait is over
	RkVoid Signal(this auto&&) noexcept;

	/// @brief Instantiates and returns the task handle.
	auto get_return_object(this auto&&) noexcept;

	/// @brief Composes and returns a new awaitable that inherits from TAwaitable to
	///		   inject behavior such as automatic tracing and task resuming.
	template<IsAwaitable TAwaitable>
	auto await_transform(TAwaitable const& in_awaitable, RUKEN_CURRENT_SOURCE_LOCATION) noexcept;

	// Coroutine lifetime
	auto initial_suspend(RUKEN_CURRENT_SOURCE_LOCATION) noexcept;
	auto final_suspend  () noexcept;
};

// Result specialization
template <typename TResult>
struct SyncTaskPromise: SyncTaskPromiseBase<TResult>
{
	std::variant<std::exception_ptr, TResult> result {};

	// Coroutine exit
	void unhandled_exception()				   noexcept;
	void return_value(TResult&&	     in_value) noexcept;
	void return_value(TResult const& in_value) noexcept
		requires (!std::is_move_assignable_v<TResult> && !std::is_move_constructible_v<TResult>);
};

// Result-less specialization
template <>
struct SyncTaskPromise<RkVoid>: SyncTaskPromiseBase<RkVoid>
{
	std::exception_ptr exception {nullptr};

	// Coroutine exit
	void unhandled_exception() noexcept;
	void return_void        () noexcept;
};

#undef RUKEN_CURRENT_SOURCE_LOCATION

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTaskPromise.inl"
