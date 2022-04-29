#pragma once

#include "Types/FundamentalTypes.hpp"

#include "Core/ExecutiveSystem/EExecutionMethod.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Processing units are things that executes tasks.
 * They do not necessarily represent a physical processor on the machine, but they rather
 * act as abstract processors. For example they can be used to implement different rendering
 * backend for the GPU, or even a way to interact with a cloud-based machine.  
 *
 * \tparam TInheriting Inheriting class, this is used for the crtp pattern.
 * \tparam TExecutionMethod Execution method of the processing unit.
 */
template <typename TInheriting, EExecutionMethod TExecutionMethod>
struct ProcessingUnit
{
    static_assert(ProcessingUnitType<TInheriting>);

    static constexpr EExecutionMethod execution_method {TExecutionMethod};
    static constexpr RkBool           is_recorded      {TExecutionMethod == EExecutionMethod::Recorded};
    static constexpr RkBool           is_immediate     {TExecutionMethod == EExecutionMethod::Immediate};
};

END_RUKEN_NAMESPACE