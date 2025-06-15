#pragma once

#include "JobSystem/Awaitables/Tasks/TaskPromise.hpp"
#include "../../Concepts/CTaskResult.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

/// Promises are allocated on the heap and need to stay alive
/// in order to retrieve their results.
/// This handle acts as a shared pointer to the promise.
template<CQueueHandle TQueueHandle, CTaskResult TResult = RkVoid>
struct Task
{
	explicit Task(TaskPromise<TQueueHandle, TResult>& in_parent) noexcept;

	Task () = default;
	Task (Task const&) noexcept;
	Task (Task&&     ) noexcept;
	~Task()			   noexcept;

	Task& operator=(Task const&) noexcept;
	Task& operator=(Task&&     ) noexcept;

	using promise_type = TaskPromise<TQueueHandle, TResult>;

	auto operator co_await() const noexcept;

	private:

		TaskPromise<TQueueHandle, TResult>*					      m_parent		     {nullptr};
		std::coroutine_handle<TaskPromise<TQueueHandle, TResult>> m_coroutine_handle {};
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/Task.inl"