#pragma once

#include "Build/Attributes.hpp"

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
 * \see StrongType
 */
struct RUKEN_EMPTY_BASES Arithmetic:
    Modulo,
    Addition,         Subtraction,
    Multiplication<>, Division<>,
    Increment,        Decrement,
    UnaryPlus,        UnaryMinus
{};

END_RUKEN_NAMESPACE