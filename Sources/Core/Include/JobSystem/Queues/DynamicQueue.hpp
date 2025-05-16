#pragma once

#include "JobSystem/WorkerInfo.hpp"
#include "JobSystem/JobSystem.hpp"

BEGIN_RUKEN_NAMESPACE

struct DynamicQueue
{
    static JobQueue& GetInstance() noexcept
    { return *WorkerInfo::current_queue; }
};

END_RUKEN_NAMESPACE