#pragma once

#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/Tasks/Task.hpp"

BEGIN_RUKEN_NAMESPACE

struct IOJobQueue : QueueHandle<IOJobQueue, 1024>
{};

template <CTaskResult TResult = RkVoid>
using IOTask = Task<IOJobQueue, TResult>;

END_RUKEN_NAMESPACE