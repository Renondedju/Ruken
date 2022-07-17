#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief An object that waits for the completion of an asynchronous event.
 * Awaiters are responsible for implementing the actual waiting algorithm.
 * This synchronization between tasks and events should not be mixed up with the content of events themselves.
 *
 * \tparam TProcessingUnit Processing unit the subscription is operating on
 */
template <ProcessingUnitType TProcessingUnit>
class Subscription
{
    using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE