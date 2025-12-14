#pragma once

#include "JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"
#include "JobSystem/Queues/DynamicQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename  TResult = RkVoid>
using DynamicTask = AsyncTask<DynamicQueue, TResult>;

END_RUKEN_NAMESPACE