#pragma once

#include "ExecutiveSystem/Concepts/CProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <CProcessingUnit TProcessingUnit>
struct QueueHandle
{
	using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE