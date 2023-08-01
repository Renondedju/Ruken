#pragma once

#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/CPUAwaitableHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Suspensions/CPUSuspension.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType>
class CPUTaskPromise;

template <QueueHandleType TQueueHandle, typename TReturnType, typename TAwaitingReturnType>
struct CPUTaskSuspension final: CPUReturningSuspension<TReturnType>
{
    using Node             = typename CPUReturningSuspension<TReturnType>::Node;
    using AwaitedReference = CPUAwaitableHandle<TReturnType>;
    using AwaitingPromise  = CPUTaskPromise<TQueueHandle, TAwaitingReturnType>;

    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_awaiting Owner of the suspension
     * \param in_awaited Awaited awaitable
     */
    CPUTaskSuspension(AwaitingPromise& in_awaiting, CPUAwaitableHandle<TReturnType> in_awaited) noexcept:
        CPUReturningSuspension {in_awaited.GetSuspension , &[in_awaiting]() noexcept {
            TQueueHandle::queue.Push(std::coroutine_handle<AwaitingPromise>::from_promise(in_awaiting));
        }, in_awaited.GetResult()}
    {}

    #pragma endregion
};

END_RUKEN_NAMESPACE