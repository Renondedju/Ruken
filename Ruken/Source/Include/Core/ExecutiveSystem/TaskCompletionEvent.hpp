#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Task completion event, in order to be implemented, each processing unit has to specialize this class
 * \tparam TProcessingUnit Processing unit type
 */
template <ProcessingUnitType TProcessingUnit>
struct TaskCompletionEvent;

END_RUKEN_NAMESPACE