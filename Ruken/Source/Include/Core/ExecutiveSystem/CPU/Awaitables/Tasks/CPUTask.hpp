#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Promises/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType = RkVoid>
struct CPUTask final: std::coroutine_handle<CPUTaskPromise<TQueueHandle, TReturnType>>
{
    using promise_type   = CPUTaskPromise<TQueueHandle, TReturnType>;
    using ProcessingUnit = typename TQueueHandle::ProcessingUnit;

    /**
     * \brief Creates a subscription to await the passed task and resume this one
     * \param in_resume_handle Task awaiting the completion of this instance
     * \return Subscription instance
     */
    auto GetSubscription() const noexcept
    {
        return this->promise().GetSubscription();
    }
};

END_RUKEN_NAMESPACE