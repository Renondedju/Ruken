#pragma once

#include <iostream>

#include "Core/Exception.hpp"
#include "JobSystem/Awaitables/SignalReceiver.hpp"
#include "JobSystem/Awaitables/Tasks/TaskAwaiter.hpp"
#include "JobSystem/Awaitables/Tasks/TaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, CTaskResult TResult>
template<typename TThis, typename TAwaitable>
auto TaskPromiseBase<TQueueHandle, TResult>::await_transform(
	this TThis&			 in_self,
	TAwaitable const&	 in_awaitable,
    std::source_location in_source_location) noexcept
{
	struct ComposedAwaitable
	{
		using TAwaiter       = decltype(std::declval<TAwaitable>().operator co_await());
		using TAwaiterResult = decltype(std::declval<TAwaiter  >().await_resume     ());

		TThis&				 task;
		TAwaitable const&    awaitable;
		std::source_location source_location;

		// This operator allows us to compose a new awaiter type
		auto operator co_await() const
		{
			struct ComposedAwaiter: TAwaiter
			{
				explicit ComposedAwaiter(TThis& in_task, TAwaitable const& in_awaitable, std::source_location const& in_source_location) noexcept:
					TAwaiter	    {in_awaitable.operator co_await()},
					task			{in_task},
					source_location {in_source_location}
				{
					TAwaiter::signal = SignalReceiver(*this);
				}

				TThis&				 task;
				std::source_location source_location;

				auto await_ready() noexcept
				{
					TRACY_END_ZONE(task.m_zone);

					return TAwaiter::await_ready();
				}

				auto await_resume() const
				{
					TRACY_BEGIN_ZONE(task.m_zone, source_location, true);

					if constexpr (std::is_same_v<TAwaiterResult, RkVoid>)
						TAwaiter::await_resume();
					else
						return TAwaiter::await_resume();
				}

				RkVoid Signal() noexcept
				{
					TQueueHandle::GetInstance().Push(std::coroutine_handle<TThis>::from_promise(task));
				}
			};

			return ComposedAwaiter(task, awaitable, source_location);
		}
	};

	return ComposedAwaitable {
		.task			 = in_self,
		.awaitable  	 = in_awaitable,
		.source_location = in_source_location
	};
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
template<typename TThis>
auto TaskPromiseBase<TQueueHandle, TResult>::initial_suspend(this TThis& in_self, std::source_location in_source_location) noexcept
{
    struct Awaiter
    {
        TThis&			     promise;
    	std::source_location source_location;

    	constexpr bool await_ready  ()						  const noexcept { return false; }
    	constexpr void await_suspend(std::coroutine_handle<>) const noexcept
    	{
    		TQueueHandle::GetInstance().Push(std::coroutine_handle<TThis>::from_promise(promise));
    	}

        void await_resume() const
        {
    		TRACY_BEGIN_ZONE(promise.m_zone, source_location, true);
        }
    };

    return Awaiter {
    	.promise         = in_self,
    	.source_location = in_source_location
    };
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
auto TaskPromiseBase<TQueueHandle, TResult>::final_suspend() noexcept
{
	struct Awaiter
	{
		TaskPromiseBase* promise;

		[[nodiscard]] bool await_ready() const noexcept {
			return promise->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1;
		}

		constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
		constexpr void await_resume ()						  const noexcept {}
	};

    return Awaiter {this};
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
void TaskPromiseBase<TQueueHandle, TResult>::unhandled_exception() noexcept
{
	std::exception_ptr ptr {std::current_exception()};

	try
	{
		std::rethrow_exception(ptr);
	}
	catch (Exception& in_exception)
	{
		std::string const what {std::format("{}", in_exception)};
		TracyMessageC(what.c_str(), what.length(), 0xFF0000);
	}

	TRACY_END_ZONE(m_zone);
	this->result = ptr;
	this->SignalConsume();
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
auto TaskPromise<TQueueHandle, TResult>::get_return_object() noexcept
{
	return Task<TQueueHandle, TResult> {*this};
}

template<CQueueHandle TQueueHandle>
auto TaskPromise<TQueueHandle, RkVoid>::get_return_object() noexcept
{
	return Task<TQueueHandle, RkVoid> {*this};
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
auto TaskPromise<TQueueHandle, TResult>::operator co_await() noexcept
{
	TaskAwaiter<TQueueHandle, TResult> awaiter {};
	awaiter = Awaitable::operator co_await();
	awaiter.promise = this;

	return awaiter;
}

template<CQueueHandle TQueueHandle>
auto TaskPromise<TQueueHandle, RkVoid>::operator co_await() noexcept
{
	TaskAwaiter<TQueueHandle, RkVoid> awaiter {};
	awaiter = Awaitable::operator co_await();
	awaiter.promise = this;

	return awaiter;
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
void TaskPromise<TQueueHandle, TResult>::return_value(TResult const& in_result) noexcept
{
	TRACY_END_ZONE(this->m_zone);

	this->result = in_result;
	this->SignalConsume();
}

template <CQueueHandle TQueueHandle>
void TaskPromise<TQueueHandle, RkVoid>::return_void() noexcept
{
	TRACY_END_ZONE(this->m_zone);

	this->result = std::exception_ptr {nullptr};
	this->SignalConsume();
}

END_RUKEN_NAMESPACE
