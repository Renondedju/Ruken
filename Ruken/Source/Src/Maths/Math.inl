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
constexpr TType Pow(TType in_value, TType in_exponent) noexcept
{
    return static_cast<TType>(std::pow(in_value, in_exponent));
}

template <ArithmeticType TType>
constexpr TType Exp(TType in_value) noexcept
{
    return static_cast<TType>(std::exp(in_value));
}

template <ArithmeticType TType>
constexpr TType Exp2(TType in_value) noexcept
{
    return static_cast<TType>(std::exp2(in_value));
}

template <ArithmeticType TType>
constexpr TType Mod(TType in_value, TType in_modulo) noexcept
{
    return static_cast<TType>(std::modf(in_value, &in_modulo));
}

template <ArithmeticType TType>
constexpr TType Frac(TType in_value) noexcept
{
    return in_value - Floor<TType>(in_value);
}

template <ArithmeticType TType>
constexpr TType Sqrt(TType in_value) noexcept
{
    return static_cast<TType>(std::sqrt(in_value));
}

template <ArithmeticType TType>
constexpr TType InvSqrt(TType in_value) noexcept
{
    return static_cast<TType>(1) / Sqrt<TType>(in_value);
}