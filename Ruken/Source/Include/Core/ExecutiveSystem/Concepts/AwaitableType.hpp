#pragma once

#include "Core/ExecutiveSystem/Awaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept AwaitableType = requires(TType in_event)
{
    static_cast<Awaitable<typename TType::ProcessingUnit>>(in_event);

    //in_event.GetSubscription({});
};

END_RUKEN_NAMESPACE