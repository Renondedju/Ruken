#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed queue handle
 *
 * This class allows the manipulation of queues as strong types,
 * statically accessible to anyone willing to use it.
 * Doing it this way avoids code duplication and allows underlying systems to use a simple
 * non templated pointer for generic operations.
 *
 * \tparam TInheriting Inheriting class (CRTP). This type makes sure the static member is unique for each handle.
 * \tparam TQueue Queue type
 */
template <typename TInheriting, ProcessingQueueType TQueue>
struct QueueHandle
{
	using ProcessingUnit = typename TQueue::ProcessingUnit;

	static inline TQueue queue {};
};

END_RUKEN_NAMESPACE