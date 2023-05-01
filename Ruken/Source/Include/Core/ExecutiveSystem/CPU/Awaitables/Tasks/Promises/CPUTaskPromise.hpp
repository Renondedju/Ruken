#pragma once

#include "Core/ExecutiveSystem/Concepts/AwaitableType.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Promises/CPUPromise.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Suspensions/CPUTaskSuspension.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType>
struct CPUTask;

template <QueueHandleType TQueueHandle, typename TReturnType>
struct CPUTaskPromise final: CPUPromise<TReturnType>
{
    using ProcessingUnit = CentralProcessingUnit;

    /// ----- Coroutine methods -----
    ///
    
    /**
     * \brief Constructs, enqueues and returns a handle to the promise
     * \return Promise handle
     */
    CPUTask<TQueueHandle, TReturnType> get_return_object()
    {
        auto handle = std::coroutine_handle<CPUTaskPromise>::from_promise(*this);

        // CPU Tasks are not processed in place and are instead pushed to a queue
        // to be picked up and processed by a worker later.
        TQueueHandle::queue.Push(handle);

        return {handle};
    }

    /**
     * \brief Converts awaited types to asynchronous events if possible
     * \tparam TAwaitable Event type
     * \param in_awaitable Asynchronous event instance
     * \return Subscription instance
     */
    template <AwaitableType TAwaitable>
    auto await_transform(TAwaitable& in_awaitable) noexcept
    {
        static_assert(std::is_same_v<typename TAwaitable::ProcessingUnit, CentralProcessingUnit>,
            "Awaiting events from other processing units is not yet supported");

        // In the case we don't need a bridge, we know the awaitable inherits from CPUAwaitable
        return CPUTaskSuspension<TQueueHandle, TReturnType> {in_awaitable.GetSuspensionNode(), *this};
    }

    RkVoid unhandled_exception() noexcept
    {
        std::terminate();
    }
};

END_RUKEN_NAMESPACE