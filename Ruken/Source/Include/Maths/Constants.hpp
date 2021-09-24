/**
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

#include <limits>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

constexpr RkFloat g_e         = 2.71828182845904523536F; // e
constexpr RkFloat g_log2_e    = 1.44269504088896340736F; // log2(e)
constexpr RkFloat g_log10_e   = 0.43429448190325182765F; // log10(e)
constexpr RkFloat g_ln2       = 0.69314718055994530942F; // ln(2)
constexpr RkFloat g_ln10      = 2.30258509299404568402F; // ln(10)
constexpr RkFloat g_pi        = 3.14159265358979323846F; // pi
constexpr RkFloat g_pi_2      = 1.57079632679489661923F; // pi/2
constexpr RkFloat g_pi_4      = 0.78539816339744830962F; // pi/4
constexpr RkFloat g_1_pi      = 0.31830988618379067154F; // 1/pi
constexpr RkFloat g_2_pi      = 0.63661977236758134308F; // 2/pi
constexpr RkFloat g_2_sqrt_pi = 1.12837916709551257390F; // 2/sqrt(pi)
constexpr RkFloat g_sqrt_2    = 1.41421356237309504880F; // sqrt(2)
constexpr RkFloat g_1_sqrt_2  = 0.70710678118654752440F; // 1/sqrt(2)

// Returns the machine epsilon, that is, the difference between 1.0 and the next value representable by the floating-point type T
constexpr RkFloat  g_float_epsilon  = std::numeric_limits<RkFloat >::epsilon();
constexpr RkDouble g_double_epsilon = std::numeric_limits<RkDouble>::epsilon();

constexpr RkFloat  g_float_infinity  = std::numeric_limits<RkFloat> ::infinity();
constexpr RkDouble g_double_infinity = std::numeric_limits<RkDouble>::infinity();

END_RUKEN_NAMESPACE