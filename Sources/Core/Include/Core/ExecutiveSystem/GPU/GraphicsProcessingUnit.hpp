#pragma once

#include "Core/ExecutiveSystem/ProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Graphics Processing Unit.
 * 
 * Implements the Vulkan Backend. Execution is deferred and requires a submission.
 */
struct GraphicsProcessingUnit final: ProcessingUnit<GraphicsProcessingUnit, EExecutionPolicy::Deferred, EInstructionType::Indirect>
{};

END_RUKEN_NAMESPACE