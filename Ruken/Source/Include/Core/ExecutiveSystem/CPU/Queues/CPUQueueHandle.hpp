#pragma once

#include "Core/ExecutiveSystem/QueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CentralProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed queue handle
 *
 * This class allows the manipulation of queues as strong types,
 * statically accessible to anyone willing to use it.
 * Doing it this way avoids code duplication and allows underlying systems to use a simple
 * non templated pointer for generic operations.
 *
 * \tparam TInheriting Inheriting class (CRTP).
 * \tparam TSize Size of the queue. Do note that this size is only
 *				 indicative and may not correspond to the underlying implementation's chosen size.
 */
template <typename TInheriting, RkSize TSize>
struct CPUQueueHandle: QueueHandle<CentralProcessingUnit>
{
    static inline CentralProcessingQueue instance {TSize};

    static CentralProcessingQueue& GetInstance() noexcept
    { return instance; }
};

END_RUKEN_NAMESPACE