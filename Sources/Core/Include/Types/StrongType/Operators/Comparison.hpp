#pragma once

#include "Build/Attributes.hpp"

#include "Types/StrongType/Operators/Comparison/Less.hpp"
#include "Types/StrongType/Operators/Comparison/Equal.hpp"
#include "Types/StrongType/Operators/Comparison/Greater.hpp"
#include "Types/StrongType/Operators/Comparison/NotEqual.hpp"
#include "Types/StrongType/Operators/Comparison/LessEqual.hpp"
#include "Types/StrongType/Operators/Comparison/GreaterEqual.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common comparison operators
 * \see StrongType
 */
struct RUKEN_EMPTY_BASES Comparison:
    Less,
    Equal,
    Greater,
    NotEqual,
    LessEqual,
    GreaterEqual
{};

END_RUKEN_NAMESPACE