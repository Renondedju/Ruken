#pragma once

#include "ExecutiveSystem/CPU/Continuations/CPUTaskContinuation.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, typename TResult>
template<typename TThis, typename TAwaitableValue>
auto CPUPromise<TQueueHandle, TResult>::await_transform(
	this TThis&							 in_self,
	CPUAwaitable<TAwaitableValue> const& in_awaitable,
    std::source_location                 in_source_location) noexcept
{
	struct Awaiter : CPUTaskContinuation<TQueueHandle, TAwaitableValue>
	{
		using Parent = CPUTaskContinuation<TQueueHandle, TAwaitableValue>;

		TThis&			     self;
		std::source_location source_location;

		explicit Awaiter(TThis&								  in_self,
						 CPUAwaitable<TAwaitableValue> const& in_handle,
						 std::source_location                 in_location) noexcept:
			Parent          {in_handle, in_self},
			self            {in_self},
			source_location {in_location}
		{}

		auto await_ready() noexcept
		{
#ifdef RUKEN_TRACE_BUILD
			TracyUtilities::TracyZoneEnd(self.m_zone);
#endif

			return Parent::await_ready();
		}

		auto await_resume() const
		{
#ifdef RUKEN_TRACE_BUILD
			self.m_zone = TracyUtilities::TracyZone(source_location, true);
#endif

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
auto CPUPromise<TQueueHandle, TResult>::initial_suspend(this TThis& in_self, std::source_location in_source_location) noexcept
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
#ifdef RUKEN_TRACE_BUILD
            auto zone = TracyUtilities::TracyZone(source_location, true);
        	promise.m_zone = zone;
#endif
        }
    };

    return Awaiter {
    	.promise         = in_self,
    	.source_location = in_source_location
    };
}

template<CQueueHandle TQueueHandle, typename TResult>
auto CPUPromise<TQueueHandle, TResult>::final_suspend() noexcept
{
	struct Awaiter
	{
		CPUPromise* promise;

		[[nodiscard]] bool await_ready() const noexcept {
			return promise->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1;
		}

		constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
		constexpr void await_resume ()						  const noexcept {}
	};

    return Awaiter {this};
}

template<CQueueHandle TQueueHandle, typename TResult>
void CPUPromise<TQueueHandle, TResult>::unhandled_exception() noexcept
{
	#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(m_zone);
	TracyMessageLC("Exception !", 0xFF0000);
	#endif

	std::exception_ptr ptr {std::current_exception()};
	this->Consume(true, &ptr);
}

template<CQueueHandle TQueueHandle, typename TResult>
auto CPUTaskPromise<TQueueHandle, TResult>::get_return_object() noexcept
{
	this->m_queue = std::addressof(TQueueHandle::GetInstance());

	return CPUTask<TQueueHandle, TResult> {*this, this->m_continuation_node};
}

template<CQueueHandle TQueueHandle>
auto CPUTaskPromise<TQueueHandle, RkVoid>::get_return_object() noexcept
{
	this->m_queue = std::addressof(TQueueHandle::GetInstance());

	return CPUTask<TQueueHandle, RkVoid> {*this, this->m_continuation_node};
}

template<CQueueHandle TQueueHandle, typename TResult>
void CPUTaskPromise<TQueueHandle, TResult>::return_value(TResult const& in_result) noexcept
{
#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(this->m_zone);
#endif

	this->Consume(true, &in_result);
}

template <CQueueHandle TQueueHandle>
void CPUTaskPromise<TQueueHandle, RkVoid>::return_void() noexcept
{
#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(this->m_zone);
#endif

	std::exception_ptr ptr {nullptr};
	this->Consume(true, &ptr);
}

END_RUKEN_NAMESPACE
