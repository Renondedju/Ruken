#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TResult = RkVoid>
struct CPUTask final: Awaitable<CentralProcessingUnit, TResult, false>, CPUAwaitableHandle<TResult, false>,
                      std::coroutine_handle<CPUTaskPromise<TQueueHandle, TResult>>
{
    using promise_type   = CPUTaskPromise<TQueueHandle, TResult>;
    using ProcessingUnit = typename TQueueHandle::ProcessingUnit;

    #pragma region Lifetime

    CPUTask() noexcept:
        CPUAwaitableHandle<TResult, false>  {nullptr},
        std::coroutine_handle<promise_type> {}
    {}

    /**
     * \brief Default constructor
     * \param in_promise Promise instance
     */
    CPUTask(promise_type& in_promise) noexcept:
        CPUAwaitableHandle<TResult, false>  {in_promise},
        std::coroutine_handle<promise_type> {std::coroutine_handle<promise_type>::from_promise(in_promise)}
    {}

    CPUTask(CPUTask const&) = default;
    CPUTask(CPUTask&&)      = default;
    ~CPUTask()              = default;

    CPUTask& operator=(CPUTask const&) = default;
    CPUTask& operator=(CPUTask&&)      = default;

    #pragma endregion
};

END_RUKEN_NAMESPACE
