#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A primitive allowing the categorization and prioritization of tasks.
 *
 * Due to the asynchronous nature of the executive system, processing units
 * don't just execute tasks right away, and use instead a level of indirection
 * called a processing queue. These queues allows the processing unit to categorize and
 * prioritize some tasks over others, to minimize latency as well as optimize throughout
 * as much as possible.
 * Some processing units even allow for the creation and configuration of custom queues
 * to add more granularity to your application if needed (ie. for path-finding, or procedural generation tasks)
 *
 * \tparam TInheriting Inheriting class, this is used for the crtp pattern.
 * \tparam TProcessingUnit Owning processing unit type of the queue
 */
template <typename TInheriting, ProcessingUnitType TProcessingUnit>
struct ProcessingQueue
{
    using ProcessingUnit = TProcessingUnit;
};

END_RUKEN_NAMESPACE