#pragma once

#include "Core/ExecutiveSystem/ProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central Processing Unit
 *
 * This class contains the thread pool required to execute the different tasks.
 * Execution is immediate (greedy) and does not require submission.
 */
struct CentralProcessingUnit final: ProcessingUnit<CentralProcessingUnit, EExecutionPolicy::Immediate, EInstructionType::Direct>
{};

END_RUKEN_NAMESPACE