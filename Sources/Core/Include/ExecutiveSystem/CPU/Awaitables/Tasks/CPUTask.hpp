#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CTaskResult.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

/// Promises are allocated on the heap and need to stay alive
/// in order to retrieve their results.
/// This handle acts as a shared pointer to the promise.
template<CQueueHandle TQueueHandle, CTaskResult TResult = RkVoid>
struct CPUTask
{
	explicit CPUTask(CPUTaskPromise<TQueueHandle, TResult>& in_parent) noexcept;

	CPUTask () = default;
	CPUTask (CPUTask const&) noexcept;
	CPUTask (CPUTask&&     ) noexcept;
	~CPUTask()			     noexcept;

	CPUTask& operator=(CPUTask const&) noexcept;
	CPUTask& operator=(CPUTask&&     ) noexcept;

	using promise_type = CPUTaskPromise<TQueueHandle, TResult>;

	auto operator co_await() const noexcept;

	private:

		CPUTaskPromise<TQueueHandle, TResult>*					     m_parent		    {nullptr};
		std::coroutine_handle<CPUTaskPromise<TQueueHandle, TResult>> m_coroutine_handle {};
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUTask.inl"