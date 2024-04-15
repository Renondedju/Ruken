
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/StrongType/Operators/Comparison/Less.hpp"
#include "Types/StrongType/Operators/Comparison/Equal.hpp"
#include "Types/StrongType/Operators/Comparison/Greater.hpp"
#include "Types/StrongType/Operators/Comparison/NotEqual.hpp"
#include "Types/StrongType/Operators/Comparison/LessEqual.hpp"
#include "Types/StrongType/Operators/Comparison/GreaterEqual.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common comparison operators
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
struct RUKEN_EMPTY_BASES Comparison:
    Less        <TStrongType>,
    Equal       <TStrongType>,
    Greater     <TStrongType>,
    NotEqual    <TStrongType>,
    LessEqual   <TStrongType>,
    GreaterEqual<TStrongType>
{};

END_RUKEN_NAMESPACE