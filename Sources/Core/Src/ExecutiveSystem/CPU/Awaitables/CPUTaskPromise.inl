#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<QueueHandleType TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult> CPUTaskPromise<TQueueHandle, TResult>::get_return_object() noexcept
{
    // The handle NEEDS to be initialized before the task is pushed
    // in the case we hold a result to make sure the reference counter
    // has time to be incremented to 1 before the task is executed and deleted by another thread
    CPUTask<TQueueHandle, TResult> handle {*this};

    return handle;
}

template<QueueHandleType TQueueHandle, typename TResult>
template<AwaitableType TAwaitable>
auto CPUTaskPromise<TQueueHandle, TResult>::await_transform(
    TAwaitable&&         in_awaitable,
    std::source_location const in_source_location) noexcept
{
    #ifdef RUKEN_TRACE_BUILD
    m_current_source_location = in_source_location;
    #endif

    using AResult              = typename std::decay_t<TAwaitable>::Result;
    using AProcessingUnit      = typename std::decay_t<TAwaitable>::ProcessingUnit;
    constexpr bool is_noexcept =          std::decay_t<TAwaitable>::reliable;
    static_assert(std::is_same_v<AProcessingUnit, CentralProcessingUnit>,
        "Awaiting events from other processing units is not yet supported");

    struct Awaiter : CPUCoroutineContinuation<AResult, is_noexcept>
    {
        CPUTaskPromise& self;

        explicit Awaiter(CPUTaskPromise& in_self, CPUAwaitableHandle<AResult, is_noexcept>&& in_handle) noexcept:
            CPUCoroutineContinuation<AResult, is_noexcept> {in_self, std::forward<CPUAwaitableHandle<AResult, is_noexcept>>(in_handle)},
            self    (in_self)
        {}

        auto await_ready() noexcept
        {
            #ifdef RUKEN_TRACE_BUILD
            TracyUtilities::TracyZoneEnd(self.m_zone);
            #endif

            return CPUCoroutineContinuation<AResult, is_noexcept>::await_ready();
        }

        auto await_suspend(std::coroutine_handle<> in_handle) noexcept
        {
            return CPUCoroutineContinuation<AResult, is_noexcept>::await_suspend(in_handle);
        }

        auto await_resume() const
        {
            #ifdef RUKEN_TRACE_BUILD
            self.m_zone = TracyUtilities::TracyZone(self.m_current_source_location, true);
            #endif

            --WorkerInfo::remaining_tasks;

            if constexpr (std::is_same_v<AResult, RkVoid>)
                CPUCoroutineContinuation<AResult, is_noexcept>::await_resume();
            else
                return CPUCoroutineContinuation<AResult, is_noexcept>::await_resume();
        }
    };

    // In the case we don't need a bridge, we know the awaitable inherits from CPUAwaitable
    if constexpr(std::is_base_of_v<CPUAwaitableHandle<AResult, is_noexcept>, TAwaitable>)
        return Awaiter {*this, std::forward<TAwaitable>(in_awaitable)};
    else
        return Awaiter {*this, CPUAwaitableHandle<AResult, is_noexcept>(in_awaitable)};
}

template<QueueHandleType TQueueHandle, typename TResult>
auto CPUTaskPromise<TQueueHandle, TResult>::initial_suspend(std::source_location const& in_location) noexcept
{
    #ifdef RUKEN_TRACE_BUILD
    m_current_source_location = in_location;
    #endif

    struct Awaiter
    {
        CPUTaskPromise& self;

        [[nodiscard]]
        bool await_ready() const noexcept
        {
            return false;//WorkerInfo::current_queue == &TQueueHandle::GetInstance() && WorkerInfo::remaining_tasks >= 1;
        }

        void await_suspend(std::coroutine_handle<>) const noexcept
        {
            // CPU Tasks are not processed in place and are instead pushed to a queue
            // to be picked up and processed by a worker later.
            TQueueHandle::GetInstance().Push(std::coroutine_handle<CPUTaskPromise>::from_promise(self));
        }

        void await_resume() const noexcept
        {
            #ifdef RUKEN_TRACE_BUILD
            self.m_zone = TracyUtilities::TracyZone(self.m_current_source_location, true);
            #endif

            --WorkerInfo::remaining_tasks;
        }
    };

    return Awaiter {*this};
}

template<QueueHandleType TQueueHandle, typename TResult>
auto CPUTaskPromise<TQueueHandle, TResult>::final_suspend() noexcept
{
    struct Awaiter: std::suspend_always
    {
        CPUTaskPromise& self;

        void await_suspend(std::coroutine_handle<>) const noexcept
        {
            #ifdef RUKEN_TRACE_BUILD
            TracyUtilities::TracyZoneEnd(self.m_zone);
            #endif

            self.SignalConsume          ();
            self.DecrementReferenceCount();
        }
    };

    return Awaiter {{}, *this};
}

template<QueueHandleType TQueueHandle, typename TResult>
void CPUTaskPromise<TQueueHandle, TResult>::unhandled_exception() noexcept
{
    #ifdef RUKEN_TRACE_BUILD
    TracyUtilities::TracyZoneEnd(m_zone);
    TracyMessageLC("Exception !", 0xFF0000);
    #endif

    this->Cancel(std::current_exception());
}

END_RUKEN_NAMESPACE
