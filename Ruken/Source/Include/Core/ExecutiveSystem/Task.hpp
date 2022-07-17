#pragma once

#include <coroutine>
#include "Core/ExecutiveSystem/Promise.hpp"
#include "Core/ExecutiveSystem/CPU/CPUTaskSubscription.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A single unit of work that may be executed asynchronously
 * \note This class only acts as a handle on the coroutine object
 * \tparam TQueue Queue type on which the task will be executed
 */
template <ProcessingQueueType TQueue>
struct Task: std::coroutine_handle<Promise<TQueue>>
{
    using promise_type    = Promise<TQueue>;
    using ProcessingQueue = TQueue;
    using ProcessingUnit  = typename TQueue::ProcessingUnit;

    /**
     * \brief Returns the task's on completion event
     * \return Completion event
     */
    explicit operator AsynchronousEvent<ProcessingUnit> const&() const noexcept
    {
        return this->promise();
    }

    template <ProcessingQueueType TAwaitingQueue> 
    auto GetSubscription(Task<TAwaitingQueue> const& in_awaiting_task) const noexcept
    {
        //return CPUTaskSubscription(this->promise().)
    }
};

END_RUKEN_NAMESPACE