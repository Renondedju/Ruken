#pragma once

#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"

BEGIN_RUKEN_NAMESPACE

struct ECSJobQueue : QueueHandle<ECSJobQueue, 1024*8>
{};

template <typename TResult>
using ECSTask = AsyncTask<ECSJobQueue, TResult>;

END_RUKEN_NAMESPACE