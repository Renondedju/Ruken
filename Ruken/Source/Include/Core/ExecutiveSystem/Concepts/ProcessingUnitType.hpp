#pragma once

#include <type_traits>

#include "Core/ExecutiveSystem/EExecutionPolicy.hpp"
#include "Core/ExecutiveSystem/EInstructionType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TInheriting, EExecutionPolicy TExecutionMethod, EInstructionType TInstructionType>
struct ProcessingUnit;

/**
 * \brief Checks if the passed type is a processing unit
 * The passed type must:
 *  - Inherit from ProcessingUnit with itself passed as the first template parameter
 *
 * \tparam TType type to check
 */
template <typename TType>
concept ProcessingUnitType = std::is_base_of_v<ProcessingUnit<TType, TType::execution_method, TType::instruction_type>, TType>;

END_RUKEN_NAMESPACE