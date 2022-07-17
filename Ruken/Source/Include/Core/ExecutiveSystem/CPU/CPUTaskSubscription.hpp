#pragma once

#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/CPUSubscription.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingQueueType TQueue>
struct CPUTaskSubscription final: CPUSubscription
{
    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_head Head node, used for insertion
     * \param in_task Task to resume when the suspension has been completed
     */
    CPUTaskSubscription(CPUSubscription::Node& in_head, Task<TQueue>&& in_task) noexcept:
        CPUSubscription {in_head},
        task            {in_task}
    {}

    CPUTaskSubscription (CPUTaskSubscription const&) = default;
    CPUTaskSubscription (CPUTaskSubscription&&)      = default;
    ~CPUTaskSubscription() override                  = default;

    #pragma endregion

    #pragma region Members

    // Task to resume when subscription got completed
    Task<TQueue> task;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the awaited event has been completed
     */
    RkVoid OnCompletion() noexcept override
    {
        TQueue::Enqueue(task);
    }

    #pragma endregion

    #pragma region Operators

    CPUTaskSubscription& operator=(CPUTaskSubscription const&) = default;
    CPUTaskSubscription& operator=(CPUTaskSubscription&&)      = default;

    #pragma endregion
};

END_RUKEN_NAMESPACE