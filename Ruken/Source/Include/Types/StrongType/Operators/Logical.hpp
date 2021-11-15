
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/StrongType/Operators/Logical/LogicalOr.hpp"
#include "Types/StrongType/Operators/Logical/LogicalAnd.hpp"
#include "Types/StrongType/Operators/Logical/LogicalNot.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common logical operators
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
struct RUKEN_EMPTY_BASES Logical:
    LogicalOr <TStrongType>,
    LogicalAnd<TStrongType>,
    LogicalNot<TStrongType>
{};

END_RUKEN_NAMESPACE