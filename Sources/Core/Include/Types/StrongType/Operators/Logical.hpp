#pragma once

#include "Build/Attributes.hpp"

#include "Types/StrongType/Operators/Logical/LogicalOr.hpp"
#include "Types/StrongType/Operators/Logical/LogicalAnd.hpp"
#include "Types/StrongType/Operators/Logical/LogicalNot.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common logical operators
 * \see StrongType
 */
struct RUKEN_EMPTY_BASES Logical:
    LogicalOr, LogicalAnd, LogicalNot
{};

END_RUKEN_NAMESPACE