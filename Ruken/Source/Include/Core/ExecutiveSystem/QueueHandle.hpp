#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingUnitType TProcessingUnit>
struct QueueHandle
{
	using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE