#pragma once

#include "JobSystem/Awaitables/SyncTask/SyncTaskPromise.hpp"
#include "JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.hpp"

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
	SyncTask 		   (SyncTask&&     ) noexcept;
	SyncTask& operator=(SyncTask const&) = delete;
	SyncTask& operator=(SyncTask&&     ) noexcept;
	~SyncTask();

	#pragma endregion

	/// @returns an awaiter that waits for the task to return or throw an exception.
	SyncTaskAwaiter<TResult> operator co_await() const noexcept;

	private:

		mutable SyncTaskResult<TResult>		m_result {};
		std::coroutine_handle<promise_type> m_handle {};
		promise_type*						m_parent {nullptr};
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTask.inl"
