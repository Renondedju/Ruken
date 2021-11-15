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

#include <cmath>

#include "Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Abs(RkFloat const in_value) noexcept
{
    return in_value < 0.0F ? -in_value : in_value;
}

RkFloat RUKEN_NAMESPACE::Ceil(RkFloat const in_value) noexcept
{
    return std::ceilf(in_value);
}

RkFloat RUKEN_NAMESPACE::Clamp(RkFloat const in_value, RkFloat const in_min, RkFloat const in_max) noexcept
{
    return in_value < in_min ? in_min : in_value > in_max ? in_max : in_value;
}

RkFloat RUKEN_NAMESPACE::Clamp01(RkFloat const in_value) noexcept
{
    return in_value < 0.0F ? 0.0F : in_value > 1.0F ? 1.0F : in_value;
}

RkFloat RUKEN_NAMESPACE::Round(RkFloat const in_value) noexcept
{
    return std::roundf(in_value);
}

RkFloat RUKEN_NAMESPACE::Floor(RkFloat const in_value) noexcept
{
    return std::floorf(in_value);
}

RkFloat RUKEN_NAMESPACE::CopySign(RkFloat const in_value, RkFloat const in_sign) noexcept
{
    return std::copysignf(in_value, in_sign);
}
