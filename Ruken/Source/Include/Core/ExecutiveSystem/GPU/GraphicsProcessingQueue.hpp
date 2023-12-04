#pragma once

#include "Core/ExecutiveSystem/ProcessingQueue.hpp"
#include "Core/ExecutiveSystem/GPU/GraphicsProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Graphics processing queue
 * \tparam TInheriting Inheriting class, this is used for the crtp pattern.
 */
template <typename TInheriting>
struct GraphicsProcessingQueue: ProcessingQueue<GraphicsProcessingUnit>
{};

END_RUKEN_NAMESPACE