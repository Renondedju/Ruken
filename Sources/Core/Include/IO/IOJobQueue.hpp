#pragma once

#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/Tasks/Task.hpp"

BEGIN_RUKEN_NAMESPACE

struct IOJobQueue : QueueHandle<IOJobQueue, 1024>
{};

#ifndef RUKEN_IO_QUEUE
#define RUKEN_IO_QUEUE IOJobQueue
#endif

template <CTaskResult TResult = RkVoid>
using IOTask = Task<RUKEN_IO_QUEUE, TResult>;

END_RUKEN_NAMESPACE