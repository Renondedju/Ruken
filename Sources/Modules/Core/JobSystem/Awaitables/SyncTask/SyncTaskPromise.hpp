#pragma once

#include "JobSystem/Queues/JobQueue.hpp"
#include "JobSystem/Awaitables/AwaitableTraits.hpp"
#include "JobSystem/Awaitables/CoroutineTracingUtils.hpp"
#include "JobSystem/Awaitables/SyncTask/SyncTaskResult.hpp"

#include <source_location>

BEGIN_RUKEN_NAMESPACE
#define RUKEN_CURRENT_SOURCE_LOCATION [[maybe_unused]] std::source_location = std::source_location::current()

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
 *	- Probably is a stackfull coroutine (there is no reliable way to measure that yet in c++26).
 *	- Unique handle that moves results instead of copying when available.
 */
template <typename TResult>
struct SyncTaskPromiseBase: CoroutineTracingUtils
{
	JobQueue*		         queue		  {nullptr};
	SyncTaskResult<TResult>* result_ptr   {nullptr};
	std::coroutine_handle<>  continuation {nullptr};

	/// @brief Operator new used to allocate coroutine body
	RkVoid* operator new(RkSize in_size);

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
	auto initial_suspend(this auto&&, RUKEN_CURRENT_SOURCE_LOCATION) noexcept;
	auto final_suspend  (this auto&) noexcept;
};

// Result specialization
template <typename TResult>
struct SyncTaskPromise: SyncTaskPromiseBase<TResult>
{
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
	// Coroutine exit
	void unhandled_exception() const noexcept;
	void return_void        () const noexcept;
};

#undef RUKEN_CURRENT_SOURCE_LOCATION

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTaskPromise.inl"
