#pragma once

#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"

BEGIN_RUKEN_NAMESPACE

struct IOJobQueue : QueueHandle<IOJobQueue, 1024>
{};

template <typename TResult = RkVoid>
using IOTask = AsyncTask<IOJobQueue, TResult>;

END_RUKEN_NAMESPACE