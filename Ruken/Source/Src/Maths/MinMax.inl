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
constexpr TType Min(TType in_lhs, TType in_rhs) noexcept
{
    return (in_lhs > in_rhs) ? in_rhs : in_lhs;
}

template <ArithmeticType TType>
constexpr TType MinExceptZero(TType in_lhs, TType in_rhs) noexcept
{
    if (in_lhs == static_cast<TType>(0))
        return in_rhs;

    return Min<TType>(in_lhs, in_rhs);
}

template <ArithmeticType TType>
constexpr TType MinExceptZero(std::initializer_list<TType> in_list) noexcept
{
    TType count = std::numeric_limits<TType>::max();
    for (TType const& element: in_list)
        count = MinExceptZero<TType>(count, element);

    return count;
}

template <ArithmeticType TType>
constexpr TType Max(TType in_lhs, TType in_rhs) noexcept
{
    return (in_lhs < in_rhs) ? in_rhs : in_lhs;
}