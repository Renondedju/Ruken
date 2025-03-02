#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUDynamicQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <CTaskResult TResult = RkVoid>
using CPUDynamicTask = CPUTask<CPUDynamicQueue, TResult>;

END_RUKEN_NAMESPACE