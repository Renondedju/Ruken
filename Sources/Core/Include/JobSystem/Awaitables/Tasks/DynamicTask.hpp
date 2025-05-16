#pragma once

#include "JobSystem/Awaitables/Tasks/Task.hpp"
#include "JobSystem/Queues/DynamicQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <CTaskResult TResult = RkVoid>
using DynamicTask = Task<DynamicQueue, TResult>;

END_RUKEN_NAMESPACE