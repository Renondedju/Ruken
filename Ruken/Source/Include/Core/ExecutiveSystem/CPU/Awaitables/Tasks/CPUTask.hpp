#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType = RkVoid>
struct CPUTask final: Awaitable<CentralProcessingUnit>, CPUAwaitableHandle<TReturnType>,
                      std::coroutine_handle<CPUTaskPromise<TQueueHandle, TReturnType>>
{
    using promise_type   = CPUTaskPromise<TQueueHandle, TReturnType>;
    using ProcessingUnit = typename TQueueHandle::ProcessingUnit;

    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_promise Promise instance
     */
    CPUTask(promise_type& in_promise) noexcept:
        Awaitable                           {},
        CPUAwaitableHandle<TReturnType>     {in_promise},
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
