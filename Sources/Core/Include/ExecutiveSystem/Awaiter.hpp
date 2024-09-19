#pragma once

#include "ExecutiveSystem/Concepts/CProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief An object that waits for the completion of an awaitable.
 * Awaiters are responsible for implementing the actual waiting algorithm.
 * This synchronization between tasks and events should not be mixed up with the content of events themselves.
 *
 * \tparam TProcessingUnit Processing unit the subscription is operating on
 */
template <CProcessingUnit TProcessingUnit>
struct Awaiter
{
	using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE