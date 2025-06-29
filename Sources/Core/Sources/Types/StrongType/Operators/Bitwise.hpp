#pragma once

#include "Build/Attributes.hpp"

#include "Types/StrongType/Operators/Bitwise/Or.hpp"
#include "Types/StrongType/Operators/Bitwise/And.hpp"
#include "Types/StrongType/Operators/Bitwise/Not.hpp"
#include "Types/StrongType/Operators/Bitwise/Xor.hpp"
#include "Types/StrongType/Operators/Bitwise/LeftShift.hpp"
#include "Types/StrongType/Operators/Bitwise/RightShift.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common bitwise operators
 * \see StrongType
 */
struct RUKEN_EMPTY_BASES Bitwise:
    Or, And, Not, Xor,
    LeftShift, RightShift
{};

END_RUKEN_NAMESPACE