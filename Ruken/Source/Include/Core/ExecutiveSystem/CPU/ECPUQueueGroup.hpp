#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief CPU Queue group
 *
 * A CPU queue can be of 2 groups:
 * - Processing: These types of queues needs to process and transform data as
 * quickly as possible and will be handled on high-performance cores if needed.
 *
 * - Transfer: Transfer queues wait a lot for peripherals like network or disk.
 * These queues are much more latency-prone and potentially way slower than a
 * processing queue but are much more power efficient.
 */
enum class ECPUQueueGroup
{
	Processing,
    Transfer
};

END_RUKEN_NAMESPACE