#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType, typename TQueue>
concept AsynchronousEventType = ProcessingQueueType<TQueue> && requires (TType in_type, TQueue const& in_queue)
{
    static_cast<AsynchronousEvent<typename TType::ProcessingUnit>>(in_type);
    in_type.template GetSubscription<TQueue>({});
};

END_RUKEN_NAMESPACE