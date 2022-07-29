#pragma once

#include "Core/ExecutiveSystem/QueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed queue handle
 *
 * This class allows the manipulation of queues as strong types,
 * statically accessible to anyone willing to use it.
 * Doing it this way avoids code duplication and allows underlying systems to use a simple
 * non templated pointer for generic operations.
 *
 * \tparam TInheriting Inheriting class (CRTP)
 * \tparam TSize Size of the queue
 * \tparam TGroup Queue group
 * \tparam TPipelinePolicy Pipeline policy
 */
template <typename TInheriting, RkSize TSize, ECPUQueueGroup TGroup, ECPUPipelinePolicy TPipelinePolicy>
struct CPUQueueHandle: QueueHandle<TInheriting, MakeCentralProcessingQueue<TSize, TGroup, TPipelinePolicy>>
{};

END_RUKEN_NAMESPACE