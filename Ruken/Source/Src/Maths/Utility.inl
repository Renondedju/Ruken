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

template <ArithmeticType TType>
constexpr TType Abs(TType in_value) noexcept
{
    return (in_value < 0) ? -in_value : in_value;
}

template <FloatingPointType TType>
constexpr TType Ceil(TType in_value) noexcept
{
    return static_cast<TType>(std::ceil(in_value));
}

template <ArithmeticType TType>
constexpr TType Clamp(TType in_value, TType in_min, TType in_max) noexcept
{
    return (in_value < in_min) ? in_min : (in_value > in_max) ? in_max : in_value;
}

template <FloatingPointType TType>
constexpr TType Clamp01(TType in_value) noexcept
{
    return (in_value < static_cast<TType>(0)) ? static_cast<TType>(0) : (in_value > static_cast<TType>(1)) ? static_cast<TType>(1) : in_value;
}

template <FloatingPointType TType>
constexpr TType Round(TType in_value) noexcept
{
    return static_cast<TType>(std::round(in_value));
}

template <FloatingPointType TType>
constexpr TType Floor(TType in_value) noexcept
{
    return static_cast<TType>(std::floor(in_value));
}

template <ArithmeticType TType>
constexpr TType CopySign(TType in_value, TType in_sign) noexcept
{
    return static_cast<TType>(std::copysign(in_value, in_sign));
}