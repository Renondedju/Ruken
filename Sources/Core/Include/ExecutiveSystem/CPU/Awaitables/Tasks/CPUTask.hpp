#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

/// Promises are allocated on the heap and need to stay alive
/// in order to retrieve their results.
/// This handle acts as a shared pointer to the promise.
template<CQueueHandle TQueueHandle, typename TResult = RkVoid>
struct CPUTask: CPUAwaitable<TPromiseAwaitableValue<TResult>>
{
	explicit CPUTask(CPUTaskPromise<TQueueHandle, TResult>&			  in_parent,
					 CPUAwaiterList<TPromiseAwaitableValue<TResult>>& in_awaiter_list) noexcept;

	CPUTask () = default;
	CPUTask (CPUTask const&) noexcept;
	CPUTask (CPUTask&&     ) noexcept;
	~CPUTask()			     noexcept;

	CPUTask& operator=(CPUTask const&) noexcept;
	CPUTask& operator=(CPUTask&&     ) noexcept;

	using promise_type = CPUTaskPromise<TQueueHandle, TResult>;

	private:

		CPUTaskPromise<TQueueHandle, TResult>*					     m_parent		    {nullptr};
		std::coroutine_handle<CPUTaskPromise<TQueueHandle, TResult>> m_coroutine_handle {};
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUTask.inl"