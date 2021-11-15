
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/StrongType/Operators/Bitwise/Or.hpp"
#include "Types/StrongType/Operators/Bitwise/And.hpp"
#include "Types/StrongType/Operators/Bitwise/Not.hpp"
#include "Types/StrongType/Operators/Bitwise/Xor.hpp"
#include "Types/StrongType/Operators/Bitwise/LeftShift.hpp"
#include "Types/StrongType/Operators/Bitwise/RightShift.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common bitwise operators
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
struct RUKEN_EMPTY_BASES Bitwise:
    Or        <TStrongType>,
    And       <TStrongType>,
    Not       <TStrongType>,
    Xor       <TStrongType>,
    LeftShift <TStrongType>,
    RightShift<TStrongType>
{};

END_RUKEN_NAMESPACE