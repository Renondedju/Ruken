#pragma once

#include "ExecutiveSystem/QueueHandle.hpp"
#include "ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

struct CPUDynamicQueue: QueueHandle<CentralProcessingUnit>
{
    static CPUQueue& GetInstance() noexcept
    { return *WorkerInfo::current_queue; }
};

END_RUKEN_NAMESPACE