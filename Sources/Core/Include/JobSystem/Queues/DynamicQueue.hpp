#pragma once

#include "JobSystem/JobSystem.hpp"
#include "JobSystem/WorkerInfo.hpp"

BEGIN_RUKEN_NAMESPACE

struct DynamicQueue
{
    static JobQueue& GetInstance() noexcept
    { return *JobSystem::worker_info.current_queue; }
};

END_RUKEN_NAMESPACE