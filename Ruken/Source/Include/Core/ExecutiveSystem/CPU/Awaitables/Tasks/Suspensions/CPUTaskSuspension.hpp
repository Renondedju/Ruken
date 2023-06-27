#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Suspensions/CPUSuspension.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType>
struct CPUTaskPromise;

template <QueueHandleType TQueueHandle, typename TReturnType>
struct CPUTaskSuspension final: CPUSuspension
{
    using PromiseType = CPUTaskPromise<TQueueHandle, TReturnType>;

    PromiseType& instance;

    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_head Head node, used for insertion
     * \param in_instance Instance of the promise
     */
    CPUTaskSuspension(Node& in_head, PromiseType& in_instance) noexcept:
        CPUSuspension {in_head},
        instance      {in_instance}
    {}

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the awaited event has been completed
     * Tasks are simply pushed back to their original queue,
     * waiting for a worker to pick it up and resume it
     */
    RkVoid OnCompletion() noexcept override
    { TQueueHandle::queue.Push(std::coroutine_handle<PromiseType>::from_promise(instance)); }

    /**
     * \brief Called when the suspension is resumed.
     * \returns The result of the awaited task
     */
    TReturnType const& await_resume() const noexcept
    { return instance.GetResult(); }

    #pragma endregion
};

template <QueueHandleType TQueueHandle>
struct CPUTaskSuspension<TQueueHandle, RkVoid> final: CPUSuspension
{
    using PromiseType = CPUTaskPromise<TQueueHandle, RkVoid>;

    PromiseType& instance;

    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_head Head node, used for insertion
     * \param in_instance Instance of the promise
     */
    CPUTaskSuspension(Node& in_head, PromiseType& in_instance) noexcept:
        CPUSuspension {in_head},
        instance      {in_instance}
    {}

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the awaited event has been completed
     * Tasks are simply pushed back to their original queue,
     * waiting for a worker to pick it up and resume it
     */
    RkVoid OnCompletion() noexcept override
    { TQueueHandle::queue.Push(std::coroutine_handle<PromiseType>::from_promise(instance)); }

    /**
     * \brief Called when the suspension is resumed.
     */
    static constexpr RkVoid await_resume() noexcept { }

    #pragma endregion
};

END_RUKEN_NAMESPACE