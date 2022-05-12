#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief An object that waits for the completion of an asynchronous event.
 *
 * Awaiters are responsible for implementing the actual waiting algorithm, that being
 * callback based, polling based or simply an injection in the recorded processing queues.
 * This synchronization between processing queues should not be mixed up with the content of events themselves.
 *
 * \tparam TOwningQueue Processing queue waiting to be notified of the completion of TAwaitedQueue
 * \tparam TAwaitedQueue Processing queue being waited on
 */
template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue>
class Subscription
{};

END_RUKEN_NAMESPACE