#pragma once

#include "Core/ExecutiveSystem/QueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

struct CPUDynamicQueue: QueueHandle<CentralProcessingUnit>
{
    static CentralProcessingQueue& GetInstance() noexcept
    { return *WorkerInfo::current_queue; }
};

END_RUKEN_NAMESPACE