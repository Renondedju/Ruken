#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Promises/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType = RkVoid>
struct CPUTask final: Awaitable<CentralProcessingUnit>, std::coroutine_handle<CPUTaskPromise<TQueueHandle, TReturnType>>
{
    using promise_type   = CPUTaskPromise<TQueueHandle, TReturnType>;
    using ProcessingUnit = typename TQueueHandle::ProcessingUnit;

    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_handle Coroutine handle
     */
    CPUTask(std::coroutine_handle<promise_type> const& in_handle) noexcept;
    CPUTask(CPUTask const&) noexcept;
    CPUTask(CPUTask&&)      noexcept;
    ~CPUTask()              noexcept;

    CPUTask& operator=(CPUTask const&) = delete;
    CPUTask& operator=(CPUTask&&)      = delete;

    #pragma endregion

    /**
     * \brief Returns the suspension node to attach to in order to wait for the task
     */
    auto& GetSuspensionNode() noexcept
    {
        return this->promise().GetSuspensionNode();
    }
};

template <QueueHandleType TQueueHandle, typename TReturnType>
CPUTask<TQueueHandle, TReturnType>::CPUTask(std::coroutine_handle<promise_type> const& in_handle) noexcept:
    Awaitable                           {},
    std::coroutine_handle<promise_type> {in_handle}
{
    in_handle.promise().AddReference();
}

template <QueueHandleType TQueueHandle, typename TReturnType>
CPUTask<TQueueHandle, TReturnType>::CPUTask(CPUTask const& in_task) noexcept:
    Awaitable                           {},
    std::coroutine_handle<promise_type> {in_task}
{
    in_task.promise().AddReference();
}

template <QueueHandleType TQueueHandle, typename TReturnType>
CPUTask<TQueueHandle, TReturnType>::CPUTask(CPUTask&& in_task) noexcept:
    Awaitable                           {},
    std::coroutine_handle<promise_type> {std::move(in_task)}
{
    in_task.promise().AddReference();
}

template <QueueHandleType TQueueHandle, typename TReturnType>
CPUTask<TQueueHandle, TReturnType>::~CPUTask() noexcept
{
     this->promise().RemoveReference();
}

template <QueueHandleType TQueueHandle>
struct CPUTask<TQueueHandle, RkVoid> final: Awaitable<CentralProcessingUnit>, std::coroutine_handle<CPUTaskPromise<TQueueHandle, RkVoid>>
{
    using promise_type   = CPUTaskPromise<TQueueHandle, RkVoid>;
    using ProcessingUnit = typename TQueueHandle::ProcessingUnit;

    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_handle Coroutine handle
     */
    CPUTask(std::coroutine_handle<promise_type> const& in_handle) noexcept
    {
        *this = in_handle;
    }

    using std::coroutine_handle<promise_type>::coroutine_handle;

    CPUTask& operator=(CPUTask const&) = delete;
    CPUTask& operator=(CPUTask&&)      = delete;

    #pragma endregion

    /**
     * \brief Returns the suspension node to attach to in order to wait for the task
     */
    auto& GetSuspensionNode() noexcept
    {
        return this->promise().GetSuspensionNode();
    }
};

END_RUKEN_NAMESPACE
