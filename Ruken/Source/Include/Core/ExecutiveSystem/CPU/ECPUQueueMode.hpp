#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines the different modes or policies for a CentralProcessingQueue
 *
 * - TimeConstrained: A time constrained queue will have the highest priority regarding execution resources.
 *   These resources will be gradually adjusted over time. TODO finish
 *
 * - Continuous queues will require at least on worker to run the queue when there is work to be done.
 *   If multiples continuous queues are required to share the same workers, those workers will jump from
 *   queue to queue in between each job to avoid stalling (at the cost of latency).
 *   This mode is best suited for resource processing, pathfinding or any other background work
 *   that should keep executing no matter what.
 *
 * - Interruptable queues will only be worked on if there is enough processing power available for it.
 *   If multiples interruptable queues are required to share the same workers, workers will chose the highest priority queue.
 *   In the case where multiple queues have the same priority, then execution time will be shared, at the cost of latency.
 *   This mode is recommended for background editor work that may need to be stopped in play-mode.
 */
enum class ECPUQueueMode
{
    TimeConstrained,
    Continuous,
    Interruptable
};

END_RUKEN_NAMESPACE