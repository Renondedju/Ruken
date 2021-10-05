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

#pragma region Static Methods

template<typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>
    ::Lerp(Vector const& in_from, Vector const& in_to, RkFloat in_ratio) noexcept
{
    return in_from + in_ratio * (in_to - in_from);
}

template<typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>
    ::Slerp(Vector const& in_from, Vector const& in_to, RkFloat const in_ratio) noexcept
{
    UnderlyingType const dot = Dot(in_from, in_to);
    Radians const theta = ArcCos(Radians(dot)) * in_ratio;

    return in_from * Cos(theta) + (in_from - in_to * dot).Normalize() * Sin(theta);
}

template<typename TMemoryLayout>
constexpr typename Vector<TMemoryLayout>::UnderlyingType Vector<TMemoryLayout>
    ::Dot(Vector const& in_lhs, Vector const& in_rhs) noexcept
{
    UnderlyingType dot = UnderlyingType(0);

    for (RkSize i = 0ULL; i < dimensions; ++i)
        dot += in_lhs.data[i] * in_rhs.data[i];

    return dot;
}

template<typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>
    ::Cross(Vector const& in_lhs, Vector const& in_rhs) noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = in_lhs.data[(i + 1) % dimensions] * in_rhs.data[(i + 2) % dimensions] -
                         in_rhs.data[(i + 1) % dimensions] * in_lhs.data[(i + 2) % dimensions];

    return result;
}

#pragma endregion

#pragma region Methods

template <typename TMemoryLayout>
constexpr typename Vector<TMemoryLayout>::UnderlyingType Vector<TMemoryLayout>
    ::Length() const noexcept
{
    return Sqrt(SqrLength());
}

template <typename TMemoryLayout>
constexpr typename Vector<TMemoryLayout>::UnderlyingType Vector<TMemoryLayout>
    ::SqrLength() const noexcept
{
    UnderlyingType sqr_length = 0;

    for (RkSize i = 0; i < dimensions; ++i)
        sqr_length += TMemoryLayout::data[i] * TMemoryLayout::data[i];

    return sqr_length;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>
    ::Normalize() noexcept
{
    UnderlyingType const length (Length());

    if (length)
    {
        for (RkSize i = 0; i < dimensions; ++i)
            TMemoryLayout::data[i] /= length;
    }

    return *this;
}

#pragma endregion

#pragma region Operators

template <typename TMemoryLayout>
constexpr RkBool Vector<TMemoryLayout>::operator==(Vector const& in_rhs) const noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        if (TMemoryLayout::data[i] != in_rhs.data[i])
            return false;

    return true;
}

template <typename TMemoryLayout>
constexpr RkBool Vector<TMemoryLayout>::operator!=(Vector const& in_rhs) const noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        if (TMemoryLayout::data[i] != in_rhs.data[i])
            return false;

    return true;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator+(Vector const& in_rhs) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] + in_rhs.data[i];

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator-(Vector const& in_rhs) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] - in_rhs.data[i];

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator*(Vector const& in_rhs) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] * in_rhs.data[i];

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator/(Vector const& in_rhs) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] / in_rhs.data[i];

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator+(UnderlyingType in_factor) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] + in_factor;

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator-(UnderlyingType in_factor) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] - in_factor;

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator*(UnderlyingType in_factor) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] * in_factor;

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator/(UnderlyingType in_factor) const noexcept
{
    Vector result;

    for (RkSize i = 0; i < dimensions; ++i)
        result.data[i] = TMemoryLayout::data[i] / in_factor;

    return result;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator+=(Vector const& in_rhs) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] += in_rhs.data[i];

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator-=(Vector const& in_rhs) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] -= in_rhs.data[i];

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator*=(Vector const& in_rhs) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] *= in_rhs.data[i];

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator/=(Vector const& in_rhs) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] /= in_rhs.data[i];

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator+=(UnderlyingType in_factor) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] += in_factor;

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator-=(UnderlyingType in_factor) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] -= in_factor;

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator*=(UnderlyingType in_factor) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] *= in_factor;

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout>& Vector<TMemoryLayout>::operator/=(UnderlyingType in_factor) noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] /= in_factor;

    return *this;
}

template <typename TMemoryLayout>
constexpr Vector<TMemoryLayout> Vector<TMemoryLayout>::operator-() const noexcept
{
    for (RkSize i = 0; i < dimensions; ++i)
        TMemoryLayout::data[i] = -TMemoryLayout::data[i];

    return *this;
}

#pragma endregion
