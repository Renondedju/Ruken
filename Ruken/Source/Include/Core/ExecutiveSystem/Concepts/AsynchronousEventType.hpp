#pragma once

#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"

BEGIN_RUKEN_NAMESPACE

// TODO: Rename me properly + cleanup
template <typename TType, typename TQueueHandle>
concept AsynchronousEventType = ProcessingQueueType<decltype(TQueueHandle::queue)>
	&& requires (TType in_event)
{
    static_cast<AsynchronousEvent<typename TType::ProcessingUnit>>(in_event);
    in_event.template GetSubscription<TQueueHandle>({});
};

END_RUKEN_NAMESPACE