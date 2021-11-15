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

#include <complex>

#include "Maths/Math.hpp"
#include "Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Pow(RkFloat const in_value, RkFloat const in_exponent) noexcept
{
    return std::powf(in_value, in_exponent);
}

RkFloat RUKEN_NAMESPACE::Exp(RkFloat const in_value) noexcept
{
    return std::expf(in_value);
}

RkFloat RUKEN_NAMESPACE::Exp2(RkFloat const in_value) noexcept
{
    return std::exp2f(in_value);
}

RkFloat RUKEN_NAMESPACE::Mod(RkFloat const in_value, RkFloat in_modulo) noexcept
{
    return std::modf(in_value, &in_modulo);
}

RkFloat RUKEN_NAMESPACE::Frac(RkFloat const in_value) noexcept
{
    return in_value - Floor(in_value);
}

RkFloat RUKEN_NAMESPACE::Sqrt(RkFloat const in_value) noexcept
{
    return std::sqrtf(in_value);
}

RkFloat RUKEN_NAMESPACE::InvSqrt(RkFloat const in_value) noexcept
{
    return 1.0F / Sqrt(in_value);
}