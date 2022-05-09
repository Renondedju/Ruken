#pragma once

#include "Core/ExecutiveSystem/EExecutionPolicy.hpp"
#include "Core/ExecutiveSystem/EInstructionType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Processing units are things that executes tasks.
 * They do not necessarily represent a physical processor on the machine, but they rather
 * act as abstract processors. For example they can be used to implement different rendering
 * backend for the GPU, or even a way to interact with a cloud-based machine.  
 *
 * \tparam TInheriting Inheriting class, this is used for the crtp pattern.
 * \tparam TExecutionMethod Execution method of the processing unit.
 * \tparam TInstructionType Instruction type of the processing unit.
 */
template <typename TInheriting, EExecutionPolicy TExecutionMethod, EInstructionType TInstructionType>
struct ProcessingUnit
{
    static constexpr EInstructionType instruction_type {TInstructionType};
    static constexpr EExecutionPolicy execution_method {TExecutionMethod};
};

END_RUKEN_NAMESPACE