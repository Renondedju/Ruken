#pragma once

#include "Core/Exception.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskAwaiter.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, typename TResult>
template<typename TThis, typename TAwaitableValue>
auto CPUTaskPromiseBase<TQueueHandle, TResult>::await_transform(
	this TThis&							 in_self,
	CPUAwaitable<TAwaitableValue> const& in_awaitable,
    std::source_location                 in_source_location) noexcept
{
	struct Awaiter : CPUTaskAwaiter<TQueueHandle, TAwaitableValue>
	{
		using Parent = CPUTaskAwaiter<TQueueHandle, TAwaitableValue>;

		TThis&			     self;
		std::source_location source_location;

		explicit Awaiter(TThis&								  in_self,
						 CPUAwaitable<TAwaitableValue> const& in_awaitable,
						 std::source_location          const& in_location) noexcept:
			Parent          {in_awaitable},
			self            {in_self},
			source_location {in_location}
		{}

		auto await_ready() noexcept
		{
			TRACY_END_ZONE(self.m_zone);

			return Parent::await_ready();
		}

		auto await_resume() const
		{
			TRACY_BEGIN_ZONE(self.m_zone, source_location, true);

			if constexpr (std::is_same_v<TAwaitableValue, RkVoid>)
				Parent::await_resume();
			else
				return Parent::await_resume();
		}
	};

	return Awaiter(in_self, in_awaitable, in_source_location);
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
