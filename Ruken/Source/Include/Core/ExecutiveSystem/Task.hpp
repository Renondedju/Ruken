#pragma once

#include <coroutine>

#include "Core/ExecutiveSystem/Promise.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A single unit of work that may be executed asynchronously
 * \note This class only acts as a handle on the coroutine object
 * \tparam TQueue Queue type on which the task will be executed
 */
template <ProcessingQueueType TQueue>
struct Task: std::coroutine_handle<Promise<TQueue>>
{
    using promise_type    = Promise<TQueue>;
    using ProcessingQueue = TQueue;
};

END_RUKEN_NAMESPACE