#pragma once

#include "JobSystem/Awaitables/SyncTask/SyncTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief A handle to a synchronous task (or lazy task).
 * @copydetails SyncTaskPromise
 *
 * @tparam TResult Result type of the task.
 */
template <typename TResult = RkVoid>
struct SyncTask
{
	using promise_type = SyncTaskPromise<TResult>;

	#pragma region Lifetime

	explicit SyncTask() = default;
	explicit SyncTask(promise_type& in_parent) noexcept;
	SyncTask 		   (SyncTask const&) = delete;
	SyncTask 		   (SyncTask&&     ) = default;
	SyncTask& operator=(SyncTask const&) = delete;
	SyncTask& operator=(SyncTask&&     ) = default;
	~SyncTask()							 = default;

	#pragma endregion

	/// @returns an awaiter that waits for the task to return or throw an exception.
	SyncTaskAwaiter<TResult> operator co_await() const noexcept;

	private:

		promise_type*						m_parent {nullptr};
		std::coroutine_handle<promise_type> m_handle {};
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTask.inl"
