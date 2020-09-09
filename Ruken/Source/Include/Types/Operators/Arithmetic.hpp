/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include "Build/Namespace.hpp"

#include "Types/Operators/Arithmetic/Modulo.hpp"
#include "Types/Operators/Arithmetic/Addition.hpp"
#include "Types/Operators/Arithmetic/Division.hpp"
#include "Types/Operators/Arithmetic/Increment.hpp"
#include "Types/Operators/Arithmetic/Decrement.hpp"
#include "Types/Operators/Arithmetic/UnaryPlus.hpp"
#include "Types/Operators/Arithmetic/UnaryMinus.hpp"
#include "Types/Operators/Arithmetic/Subtraction.hpp"
#include "Types/Operators/Arithmetic/Multiplication.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common arithmetic operators
 *
 * \tparam TStrongTypedef Base NamedType
 *
 * \see NamedType
 */
template <typename TStrongTypedef>
struct Arithmetic:  Modulo          <TStrongTypedef>,
                    Addition        <TStrongTypedef>,
                    Division        <TStrongTypedef>,
                    Increment       <TStrongTypedef>,
                    Decrement       <TStrongTypedef>,
                    UnaryPlus       <TStrongTypedef>,
                    UnaryMinus      <TStrongTypedef>,
                    Subtraction     <TStrongTypedef>,
                    Multiplication  <TStrongTypedef>
{};

END_RUKEN_NAMESPACE