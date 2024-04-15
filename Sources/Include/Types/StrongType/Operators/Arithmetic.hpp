
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/StrongType/Operators/Arithmetic/Modulo.hpp"
#include "Types/StrongType/Operators/Arithmetic/Addition.hpp"
#include "Types/StrongType/Operators/Arithmetic/Division.hpp"
#include "Types/StrongType/Operators/Arithmetic/Increment.hpp"
#include "Types/StrongType/Operators/Arithmetic/Decrement.hpp"
#include "Types/StrongType/Operators/Arithmetic/UnaryPlus.hpp"
#include "Types/StrongType/Operators/Arithmetic/UnaryMinus.hpp"
#include "Types/StrongType/Operators/Arithmetic/Subtraction.hpp"
#include "Types/StrongType/Operators/Arithmetic/Multiplication.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common arithmetic operators
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
struct RUKEN_EMPTY_BASES Arithmetic:
    Modulo        <TStrongType>,
    Addition      <TStrongType>,
    Division      <TStrongType>,
    Increment     <TStrongType>,
    Decrement     <TStrongType>,
    UnaryPlus     <TStrongType>,
    UnaryMinus    <TStrongType>,
    Subtraction   <TStrongType>,
    Multiplication<TStrongType>
{};

END_RUKEN_NAMESPACE