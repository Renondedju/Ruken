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

#pragma region Operators

// For some reason, MSVC considers that if this if constexpr is never used, the following return statement is unreachable code
#pragma warning(push)
#pragma warning(disable : 4702) // Unreachable code

template <EAngleUnit TUnitType, ArithmeticType TType>
template <ArithmeticType TOtherType>
constexpr Angle<TUnitType, TType>::operator Angle<EAngleUnit::Degree, TOtherType>() const noexcept
{
    TOtherType value = static_cast<TOtherType>(*this);

    if constexpr (TUnitType == EAngleUnit::Radian)
        return Angle<EAngleUnit::Degree, TOtherType>(value * static_cast<TOtherType>(180.0 / g_pi));

    return Angle<EAngleUnit::Degree, TOtherType>(value);
}

template <EAngleUnit TUnitType, ArithmeticType TType>
template <ArithmeticType TOtherType>
constexpr Angle<TUnitType, TType>::operator Angle<EAngleUnit::Radian, TOtherType>() const noexcept
{
    TOtherType value = static_cast<TOtherType>(*this);

    if constexpr (TUnitType == EAngleUnit::Degree)
        return Angle<EAngleUnit::Radian, TOtherType>(value * static_cast<TOtherType>(g_pi / 180.0F));

    return Angle<EAngleUnit::Radian, TOtherType>(value);
}

#pragma warning(pop)
#pragma endregion
