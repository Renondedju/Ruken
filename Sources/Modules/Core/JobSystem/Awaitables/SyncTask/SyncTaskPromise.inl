#pragma once

#include "Core/JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
RkVoid* SyncTaskPromiseBase<TResult>::operator new(RkSize const in_size)
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return ::operator new(in_size);
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::operator co_await(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return SyncTaskAwaiter<TResult>(&in_self);
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::get_return_object(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return SyncTask<TResult>(in_self);
}

template<typename TResult>
template<IsAwaitable TAwaitable>
auto SyncTaskPromiseBase<TResult>::await_transform(
	TAwaitable const&    in_awaitable,
	std::source_location in_source_location) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	using TAwaiter = decltype(std::declval<TAwaitable>().operator co_await());

	AwaitTransformAwaiter<TAwaiter> awaiter {
		in_awaitable.operator co_await(),
		this, in_source_location
	};

	// When a wait is asynchronous, the execution of the coroutine will not be resumed automatically.
	// Instead, we need to install a callback to be notified of the event and schedule the coroutine back for execution.
	if constexpr (IsAsyncAwaitable<TAwaitable>)
	{
		awaiter.signal = SignalReceiver(*this);
		queue		   = JobSystem::worker_info.current_queue;
	}

	return awaiter;
}

template<typename TResult>
RkVoid SyncTaskPromiseBase<TResult>::Signal(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// When an asynchronous wait is over we need to schedule the coroutine back for execution
	in_self.queue->Push(std::coroutine_handle<decltype(in_self)>::from_promise(in_self));
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::initial_suspend(
	this auto&&				   in_self,
	std::source_location const in_source_location) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Coroutine is started when waited for
	return InitialSuspendAwaiter<std::suspend_always> {
		.promise        = &in_self,
		.start_location = in_source_location
	};
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::final_suspend() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	struct Awaiter: std::suspend_always
	{
		SyncTaskPromiseBase* promise;

		std::coroutine_handle<> await_suspend(std::coroutine_handle<> const in_coro) const noexcept
		{
			// Returning a coroutine handle in final suspend only suspends the coroutine without destroying it.
			// We need to do it manually to avoid leaks.
			in_coro.destroy();

			return promise->continuation;
		}
	};

	return FinalSuspendAwaiter<Awaiter> {
		Awaiter {{}, this}, this
	};
}

template<typename TResult>
void SyncTaskPromise<TResult>::unhandled_exception() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	this->ReportException(ptr);

	if (this->result_ptr)
		*this->result_ptr = ptr;
}

inline void SyncTaskPromise<RkVoid>::unhandled_exception() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	ReportException(ptr);

	if (result_ptr)
		*result_ptr = ptr;
}

inline void SyncTaskPromise<RkVoid>::return_void() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (result_ptr)
		*result_ptr = nullptr;
}

template<typename TResult>
void SyncTaskPromise<TResult>::return_value(TResult&& in_value) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (this->result_ptr)
		this->result_ptr->template emplace<TResult>(std::forward<TResult>(in_value));
}

template<typename TResult>
void SyncTaskPromise<TResult>::return_value(TResult const& in_value) noexcept
	requires (!std::is_move_assignable_v<TResult> && !std::is_move_constructible_v<TResult>)
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (this->result_ptr)
		*this->result_ptr = std::forward<TResult>(in_value);
}

END_RUKEN_NAMESPACE
