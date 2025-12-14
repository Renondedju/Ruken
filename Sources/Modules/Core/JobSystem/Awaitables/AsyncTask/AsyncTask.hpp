#pragma once

#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTaskPromise.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief A handle to an asynchronous task (or greedy task)
 * @copydetails AsyncTaskPromise
 *
 * @tparam TQueueHandle Queue to execute the task on.
 * @tparam TResult Result type of the task.
 */
template<IsQueueHandle TQueueHandle, typename TResult = RkVoid>
struct AsyncTask
{
	using promise_type = AsyncTaskPromise<TQueueHandle, TResult>;

	#pragma region Lifetime

	explicit AsyncTask() = default;
	explicit AsyncTask(AsyncTaskPromise<TQueueHandle, TResult>& in_parent) noexcept;
	AsyncTask 		    (AsyncTask const&) noexcept;
	AsyncTask 		    (AsyncTask&&     ) noexcept;
	AsyncTask& operator=(AsyncTask const&) noexcept;
	AsyncTask& operator=(AsyncTask&&     ) noexcept;
	~AsyncTask()						   noexcept;

	#pragma endregion

	/// @returns True if the task is done.
	RkBool Done() const noexcept;

	/// @returns an awaiter that waits for the task to return or throw an exception.
	auto operator co_await() const noexcept;

	private:

		AsyncTaskPromise<TQueueHandle, TResult>*					   m_parent {nullptr};
		std::coroutine_handle<AsyncTaskPromise<TQueueHandle, TResult>> m_handle {};
};

END_RUKEN_NAMESPACE

#include "AsyncTask.inl"
