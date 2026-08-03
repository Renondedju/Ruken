#pragma once

#include "Core/JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.hpp"
#include "Build/BuildInfo.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::operator co_await(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return SyncTaskAwaiter<TResult> {
		std::move(in_self.ManualResetEvent::operator co_await()), &in_self
	};
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::get_return_object(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	in_self.queue = JobSystem::worker_info.current_queue;

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
	// Instead, we need to install a callback to be notified of the event and resume the coroutine.
	if constexpr (IsAsyncAwaitable<TAwaitable>)
		awaiter.signal = SignalReceiver(*this);

	return awaiter;
}

template<typename TResult>
RkVoid SyncTaskPromiseBase<TResult>::Signal(this auto&& in_self) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// When an asynchronous wait is over coroutine is scheduled back to its original queue.
	in_self.queue->Push(std::coroutine_handle<decltype(in_self)>::from_promise(in_self));
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::initial_suspend(std::source_location const in_source_location) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Coroutine is run in place
	return InitialSuspendAwaiter< std::suspend_never> {
		{}, this, in_source_location
	};
}

template<typename TResult>
auto SyncTaskPromiseBase<TResult>::final_suspend() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Coroutine is reference counted for destruction
	struct Awaiter
	{
		[[nodiscard]]
		static RkBool await_ready () noexcept { return false; }
		static RkVoid await_resume() noexcept {}

		RkVoid await_suspend(std::coroutine_handle<SyncTaskPromise<TResult>> in_handle) const noexcept
		{
			if (in_handle.promise().references.fetch_sub(1, std::memory_order_release) == 1)
			{
				std::atomic_thread_fence(std::memory_order_acquire);
				in_handle.destroy();
			}
		}
	};

	return FinalSuspendAwaiter<Awaiter> {
		Awaiter {}, this
	};
}

template<typename TResult>
void SyncTaskPromise<TResult>::unhandled_exception() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	this->ReportException(ptr);
	this->result = ptr;
	this->SignalConsume();
}

inline void SyncTaskPromise<RkVoid>::unhandled_exception() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::exception_ptr const ptr {std::current_exception()};
	ReportException(ptr);
	exception = ptr;
	SignalConsume();
}

inline void SyncTaskPromise<RkVoid>::return_void() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	exception = nullptr;
	SignalConsume();
}

template<typename TResult>
void SyncTaskPromise<TResult>::return_value(TResult&& in_value) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	this->result.template emplace<TResult>(std::move(in_value));
	this->SignalConsume();
}

template<typename TResult>
void SyncTaskPromise<TResult>::return_value(TResult const& in_value) noexcept
	requires (!std::is_move_assignable_v<TResult> && !std::is_move_constructible_v<TResult>)
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	this->result = in_value;
	this->SignalConsume();
}

END_RUKEN_NAMESPACE
