#pragma once

#include <coroutine>
#include "Core/ExecutiveSystem/Promise.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A single unit of work that may be executed asynchronously
 * \note This class only acts as a handle on the coroutine object
 * \tparam TQueueHandle Strong typed handle to the queue on which the task will be executed
 */
template <QueueHandleType TQueueHandle>
struct Task: std::coroutine_handle<Promise<TQueueHandle>>
{
    using promise_type    = Promise<TQueueHandle>;
    using ProcessingUnit  = typename TQueueHandle::ProcessingUnit;

    /**
     * \brief Creates a subscription to await the passed task and resume this one
     * \tparam TAwaitingQueue Queue the awaiting task lives in
     * \param in_awaiting_task Task awaiting the completion of this instance
     * \return Subscription instance
     */
    template <QueueHandleType TAwaitingQueue> 
    auto GetSubscription(Task<TAwaitingQueue>&& in_awaiting_task) const noexcept
    {
        return this->promise().GetSubscription(std::forward<Task<TAwaitingQueue>>(in_awaiting_task));
    }
};

END_RUKEN_NAMESPACE