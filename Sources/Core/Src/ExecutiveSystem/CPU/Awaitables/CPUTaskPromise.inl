#pragma once

#include <iostream>

#include "Core/Exception.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskAwaiter.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, typename TResult>
template<typename TThis, typename TAwaitable>
auto CPUTaskPromiseBase<TQueueHandle, TResult>::await_transform(
	this TThis&			 in_self,
	TAwaitable const&	 in_awaitable,
    std::source_location in_source_location) noexcept
{
	struct TaskAwaitable
	{
		using TAwaiter      = std::remove_cvref_t<decltype(std::declval<TAwaitable>().operator co_await())>;
		using TAwaiterValue = typename TAwaiter::SignalValue;

		TThis&				 task;
		TAwaitable const&    awaitable;
		std::source_location source_location;

		// This operator allows us to compose a new awaiter type
		auto operator co_await() const
		{
			struct TaskAwaiter : CPUTaskAwaiter<TQueueHandle, TAwaiterValue, TAwaiter>
			{
				explicit TaskAwaiter(TAwaitable const& in_awaitable, TThis& in_task, std::source_location const& in_source_location):
					CPUTaskAwaiter<TQueueHandle, TAwaiterValue> {in_awaitable.operator co_await()},
					task			{in_task},
					source_location {in_source_location}
				{}

				TThis&				 task;
				std::source_location source_location;

				auto await_ready() noexcept
				{
					TRACY_END_ZONE(task.m_zone);

					return CPUTaskAwaiter<TQueueHandle, TAwaiterValue>::await_ready();
				}

				auto await_resume() const
				{
					TRACY_BEGIN_ZONE(task.m_zone, source_location, true);

					if constexpr (std::is_same_v<TAwaiterValue, RkVoid>)
						CPUTaskAwaiter<TQueueHandle, TAwaiterValue>::await_resume();
					else
						return CPUTaskAwaiter<TQueueHandle, TAwaiterValue>::await_resume();
				}
			};

			return TaskAwaiter(awaitable, task, source_location);
		}
	};

	return TaskAwaitable {
		.task			 = in_self,
		.awaitable  	 = in_awaitable,
		.source_location = in_source_location
	};
}

template<CQueueHandle TQueueHandle, typename TResult>
template<typename TThis>
auto CPUTaskPromiseBase<TQueueHandle, TResult>::initial_suspend(this TThis& in_self, std::source_location in_source_location) noexcept
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

template<CQueueHandle TQueueHandle, typename TResult>
auto CPUTaskPromiseBase<TQueueHandle, TResult>::final_suspend() noexcept
{
	struct Awaiter
	{
		CPUTaskPromiseBase* promise;

		[[nodiscard]] bool await_ready() const noexcept {
			return promise->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1;
		}

		constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
		constexpr void await_resume ()						  const noexcept {}
	};

    return Awaiter {this};
}

template<CQueueHandle TQueueHandle, typename TResult>
void CPUTaskPromiseBase<TQueueHandle, TResult>::unhandled_exception() noexcept
{
	std::exception_ptr ptr {std::current_exception()};

	try
	{
		std::rethrow_exception(ptr);
	}
	catch (Exception& in_exception)
	{
		std::string const what {in_exception};
		TracyMessageC(what.c_str(), what.length(), 0xFF0000);
	}

	TRACY_END_ZONE(m_zone);
	this->Consume(true, &ptr);
}

template<CQueueHandle TQueueHandle, typename TResult>
auto CPUTaskPromise<TQueueHandle, TResult>::get_return_object() noexcept
{
	return CPUTask<TQueueHandle, TResult> {*this, this->m_awaiter_list};
}

template<CQueueHandle TQueueHandle>
auto CPUTaskPromise<TQueueHandle, RkVoid>::get_return_object() noexcept
{
	return CPUTask<TQueueHandle, RkVoid> {*this, this->m_awaiter_list};
}

template<CQueueHandle TQueueHandle, typename TResult>
void CPUTaskPromise<TQueueHandle, TResult>::return_value(TResult const& in_result) noexcept
{
	TRACY_END_ZONE(this->m_zone);

	this->Consume(true, &in_result);
}

template <CQueueHandle TQueueHandle>
void CPUTaskPromise<TQueueHandle, RkVoid>::return_void() noexcept
{
	TRACY_END_ZONE(this->m_zone);

	std::exception_ptr ptr {nullptr};
	this->Consume(true, &ptr);
}

END_RUKEN_NAMESPACE
