#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUDynamicQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult = RkVoid>
using CPUDynamicTask = CPUTask<CPUDynamicQueue, TResult>;

END_RUKEN_NAMESPACE