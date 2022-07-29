#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Asynchronous event that can be waited on and subscribed to.
 * Events implements the "when" question, they are responsible for designating
 * a given point in time until an awaiter is supposed to wait.
 *
 * \tparam TProcessingUnit Queue type the event lives in
 */
template <ProcessingUnitType TProcessingUnit>
struct AsynchronousEvent
{
    using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE