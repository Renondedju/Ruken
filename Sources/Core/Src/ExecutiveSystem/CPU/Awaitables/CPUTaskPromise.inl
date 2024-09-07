#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"
#include "ExecutiveSystem/CPU/WorkerInfo.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
template<typename TAwaitableValue>
auto CPUPromise<TResult>::await_transform(CPUAwaitable<TAwaitableValue> const& in_awaitable,
                                          std::source_location                 in_source_location) noexcept
{
	struct Awaiter : CPUCoroutineContinuation<TAwaitableValue>
	{
		using Parent = CPUCoroutineContinuation<TAwaitableValue>;

		CPUPromise&			 self;
		std::source_location source_location;

		explicit Awaiter(CPUPromise&						  in_self,
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

			--WorkerInfo::remaining_tasks;

			if constexpr (std::is_same_v<TAwaitableValue, RkVoid>)
				Parent::await_resume();
			else
				return Parent::await_resume();
		}
	};

	return Awaiter(*this, in_awaitable, in_source_location);
}

template<typename TResult>
auto CPUPromise<TResult>::initial_suspend(std::source_location in_source_location) noexcept
{
    struct Awaiter : std::suspend_always
    {
        CPUPromise&			 self;
    	std::source_location source_location;

        void await_resume() const noexcept
        {
            #ifdef RUKEN_TRACE_BUILD
            self.m_zone = TracyUtilities::TracyZone(source_location, true);
            #endif

            --WorkerInfo::remaining_tasks;
        }
    };

    return Awaiter {{}, *this, in_source_location};
}

template<typename TResult>
auto CPUPromise<TResult>::final_suspend() noexcept
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

template<typename TResult>
void CPUPromise<TResult>::unhandled_exception() noexcept
{
	#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(m_zone);
	TracyMessageLC("Exception !", 0xFF0000);
	#endif

	std::exception_ptr ptr {std::current_exception()};
	this->Consume(true, &ptr);
}

template<typename TQueueHandle, typename TResult>
auto CPUTaskPromise<TQueueHandle, TResult>::get_return_object() noexcept
{
	CPUQueue&				       queue {TQueueHandle::GetInstance()};
	CPUTask<TQueueHandle, TResult> task  {*this, this->m_continuation_node};

	queue.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));
	this->m_queue = std::addressof(queue);

	return task;
}

template<typename TQueueHandle>
auto CPUTaskPromise<TQueueHandle, RkVoid>::get_return_object() noexcept
{
	CPUQueue&				      queue {TQueueHandle::GetInstance()};
	CPUTask<TQueueHandle, RkVoid> task  {*this, this->m_continuation_node};

	queue.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));
	this->m_queue = std::addressof(queue);

	return task;
}

template<typename TQueueHandle, typename TResult>
void CPUTaskPromise<TQueueHandle, TResult>::return_value(TResult const& in_result) noexcept
{
#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(this->m_zone);
#endif

	this->Consume(true, &in_result);
}

template <typename TQueueHandle>
void CPUTaskPromise<TQueueHandle, RkVoid>::return_void() noexcept
{
#ifdef RUKEN_TRACE_BUILD
	TracyUtilities::TracyZoneEnd(this->m_zone);
#endif

	std::exception_ptr ptr {nullptr};
	this->Consume(true, &ptr);
}

END_RUKEN_NAMESPACE
