#pragma once

#include "JobSystem/Awaitables/SignalReceiver.hpp"
#include "JobSystem/Awaitables/AsyncTask/AsyncTaskAwaiter.hpp"

BEGIN_RUKEN_NAMESPACE

template<IsQueueHandle TQueueHandle, typename TResult>
template<IsAwaitable TAwaitable>
auto AsyncTaskPromiseBase<TQueueHandle, TResult>::await_transform(
	TAwaitable const&	 in_awaitable,
    std::source_location in_source_location) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	using TAwaiter = decltype(std::declval<TAwaitable>().operator co_await());

	AwaitTransformAwaiter<TAwaiter> awaiter {
		std::move(in_awaitable.operator co_await()),
		this, in_source_location
	};

	// When a wait is asynchronous, the execution of the coroutine will not be resumed automatically.
	// Instead, we need to install a callback to be notified of the event and schedule the coroutine back for execution.
	if constexpr (IsAsyncAwaitable<TAwaitable>)
		awaiter.signal = SignalReceiver(*this);

	return awaiter;
}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTaskPromiseBase<TQueueHandle, TResult>::initial_suspend(this auto&& in_self, std::source_location in_source_location) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Coroutine is scheduled when instantiated
    struct Awaiter: std::suspend_always
    {
        std::coroutine_handle<> handle;

    	RkVoid await_suspend(std::coroutine_handle<>) const noexcept {
    		TQueueHandle::GetInstance().Push(handle);
    	}
    };

	return InitialSuspendAwaiter<Awaiter> {
		Awaiter {{}, std::coroutine_handle<decltype(in_self)>::from_promise(in_self)},
		&in_self, in_source_location
	};
}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTaskPromiseBase<TQueueHandle, TResult>::final_suspend() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Coroutine is reference counted for destruction
	struct Awaiter: std::suspend_always
	{
		AsyncTaskPromiseBase* promise;

		RkBool await_ready() const noexcept {
			return promise->references.fetch_sub(1, std::memory_order_acq_rel) == 1;
		}
	};

	return FinalSuspendAwaiter<Awaiter> {
		Awaiter {{}, this}, this
	};
}

template<IsQueueHandle TQueueHandle, typename TResult>
RkVoid AsyncTaskPromiseBase<TQueueHandle, TResult>::Signal(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// When an asynchronous wait is over we need to schedule the coroutine back for execution
	TQueueHandle::GetInstance().Push(
		std::coroutine_handle<decltype(in_self)>::from_promise(in_self)
	);
}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTaskPromiseBase<TQueueHandle, TResult>::get_return_object(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return AsyncTask<TQueueHandle, TResult> {in_self};
}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTaskPromiseBase<TQueueHandle, TResult>::operator co_await(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return AsyncTaskAwaiter<TQueueHandle, TResult> {
		std::move(in_self.ManualResetEvent::operator co_await()), &in_self
	};
}

template<IsQueueHandle TQueueHandle, typename TResult>
void AsyncTaskPromise<TQueueHandle, TResult>::unhandled_exception() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	this->ReportException(ptr);

	result = ptr;
	std::ignore = this->SignalConsume();
}

template<IsQueueHandle TQueueHandle, typename TResult>
void AsyncTaskPromise<TQueueHandle, TResult>::return_value(TResult const& in_result) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	result = in_result;
	this->SignalConsume();
}

template<IsQueueHandle TQueueHandle, typename TResult>
void AsyncTaskPromise<TQueueHandle, TResult>::return_value(TResult&& in_result) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	result = std::forward<TResult>(in_result);
	this->SignalConsume();
}

template<IsQueueHandle TQueueHandle>
void AsyncTaskPromise<TQueueHandle, RkVoid>::unhandled_exception() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	this->ReportException(ptr);

	exception = ptr;
	std::ignore = this->SignalConsume();
}

template <IsQueueHandle TQueueHandle>
void AsyncTaskPromise<TQueueHandle, RkVoid>::return_void() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	exception = nullptr;
	this->SignalConsume();
}

END_RUKEN_NAMESPACE
