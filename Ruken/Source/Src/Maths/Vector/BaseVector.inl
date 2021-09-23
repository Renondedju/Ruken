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

#pragma region Methods

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::Lerp(BaseVector const& in_from, BaseVector const& in_to, RkFloat in_ratio) noexcept
{
    return in_from + in_ratio * (in_to - in_from);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::Slerp(BaseVector const& in_from, BaseVector const& in_to, float in_ratio) noexcept
{
    TType const dot   = Dot(in_from, in_to);
    TType const theta = acos(dot) * in_ratio;

    TDerived relative_vec = in_from - in_to * dot;

    relative_vec.Normalize();

    return in_from * cos(theta) + relative_vec * sin(theta);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType BaseVector<TDerived, TLength, TType>
    ::Dot(BaseVector const& in_lhs, BaseVector const& in_rhs) noexcept
{
    return in_lhs.Dot(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::Cross(BaseVector const& in_lhs, BaseVector const& in_rhs) noexcept
{
    return in_lhs.Cross(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType& BaseVector<TDerived, TLength, TType>
    ::At(RkSize in_index) noexcept
{
    return static_cast<TDerived&>(*this).data[in_index];
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType const& BaseVector<TDerived, TLength, TType>
    ::At(RkSize in_index) const noexcept
{
    return static_cast<TDerived const&>(*this).data[in_index];
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType BaseVector<TDerived, TLength, TType>
    ::SqrLength() const noexcept
{
    TDerived const& derived = static_cast<TDerived const&>(*this);
    RkSize sqr_length = 0;

    for (RkSize i = 0; i < TLength; ++i)
        sqr_length += derived.data[i] * derived.data[i];

    return sqr_length;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType BaseVector<TDerived, TLength, TType>
    ::Length() const noexcept
{
    return sqrt(SqrLength());
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Normalize() noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    TType const length (Length());

    if (length)
    {
        for (RkSize i = 0; i < TLength; i++)
            derived.data[i] /= length;
    }

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::GetNormalized() const noexcept
{
    return TDerived(*this).Normalize();
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Add(BaseVector const& in_other) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] += in_other.At(i);

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Subtract(BaseVector const& in_other) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] -= in_other.At(i);

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Multiply(BaseVector const& in_other) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] *= in_other.At(i);

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Divide(BaseVector const& in_other) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] = (in_other.At(i) != 0) ? derived.data[i] / in_other.At(i) : 0;

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Add(TType in_value) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] += in_value;

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Subtract(TType in_value) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] -= in_value;

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Multiply(TType in_value) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] *= in_value;

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::Divide(TType in_value) noexcept
{
    TDerived& derived = static_cast<TDerived&>(*this);
    for (RkSize i = 0; i < TLength; i++)
        derived.data[i] = (in_value != 0) ? derived.data[i] / in_value : 0;

    return *this;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr bool BaseVector<TDerived, TLength, TType>
    ::IsEqual(BaseVector const& in_other) const noexcept
{
    TDerived const& derived = static_cast<TDerived const&>(*this);
    for (RkSize i = 0; i < TLength; i++)
    {
        if (derived.data[i] != in_other.At(i))
            return false;
    }

    return true;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr bool BaseVector<TDerived, TLength, TType>
    ::IsNotEqual(BaseVector const& in_other) const noexcept
{
    TDerived const& derived = static_cast<TDerived const&>(*this);
    for (RkSize i = 0; i < TLength; i++)
    {
        if (derived.data[i] == in_other.At(i))
            return false;
    }

    return true;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType BaseVector<TDerived, TLength, TType>
    ::Dot(BaseVector const& in_other) const noexcept
{
    TDerived const& derived = static_cast<TDerived const&>(*this);
    TType dot = 0;

    for (RkSize i = 0; i < TLength; i++)
        dot += derived.data[i] * in_other.At(i);

    return dot;
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::Cross(BaseVector const& in_other) const noexcept
{
    TDerived const& derived = static_cast<TDerived const&>(*this);
    BaseVector result;

    for (RkSize i = 0; i < TLength; i++)
        result.At(i) =  derived.data[(i + 1) % TLength] * in_other.At((i + 2) % TLength) -
                        in_other.At((i + 1) % TLength) * derived.data[(i + 2) % TLength];

    return result;
}

#pragma endregion

#pragma region Operators

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr bool BaseVector<TDerived, TLength, TType>
    ::operator==(BaseVector const& in_rhs) const noexcept
{
    return IsEqual(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr bool BaseVector<TDerived, TLength, TType>
    ::operator!=(BaseVector const& in_rhs) const noexcept
{
    return IsNotEqual(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator+(BaseVector const& in_rhs) const noexcept
{
    return BaseVector(*this).Add(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator-(BaseVector const& in_rhs) const noexcept
{
    return BaseVector(*this).Subtract(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator*(BaseVector const& in_rhs) const noexcept
{
    return BaseVector(*this).Multiply(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator/(BaseVector const& in_rhs) const noexcept
{
    return BaseVector(*this).Divide(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator+(TType in_factor) const noexcept
{
    return BaseVector(*this).Add(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator-(TType in_factor) const noexcept
{
    return BaseVector(*this).Subtract(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator*(TType in_factor) const noexcept
{
    return BaseVector(*this).Multiply(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator/(TType in_factor) const noexcept
{
    return BaseVector(*this).Divide(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator+=(BaseVector const& in_rhs) noexcept
{
    return Add(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator-=(BaseVector const& in_rhs) noexcept
{
    return Subtract(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator*=(BaseVector const& in_rhs) noexcept
{
    return Multiply(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator/=(BaseVector const& in_rhs) noexcept
{
    return Divide(in_rhs);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator+=(TType in_factor) noexcept
{
    return Add(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator-=(TType in_factor) noexcept
{
    return Subtract(in_factor);
}
template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator*=(TType in_factor) noexcept
{
    return Multiply(in_factor);
}
template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType>& BaseVector<TDerived, TLength, TType>
    ::operator/=(TType in_factor) noexcept
{
    return Divide(in_factor);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType const& BaseVector<TDerived, TLength, TType>
    ::operator[](RkSize const in_index) const noexcept
{
    return At(in_index);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr TType& BaseVector<TDerived, TLength, TType>
    ::operator[](RkSize const in_index) noexcept
{
    return At(in_index);
}

template <typename TDerived, RkSize TLength, ArithmeticType TType>
constexpr BaseVector<TDerived, TLength, TType> BaseVector<TDerived, TLength, TType>
    ::operator-() const noexcept
{
    return BaseVector(*this).Multiply(-1);
}

#pragma endregion 