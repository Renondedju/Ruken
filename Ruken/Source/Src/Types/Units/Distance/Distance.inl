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

template <>
constexpr Distance<EDistanceUnit::Centimeters>::operator Distance<EDistanceUnit::Kilometers>() const noexcept
{
    return Distance<EDistanceUnit::Kilometers>(static_cast<UnderlyingType>(*this) / 100'000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Meters>::operator Distance<EDistanceUnit::Kilometers>() const noexcept
{
    return Distance<EDistanceUnit::Kilometers>(static_cast<UnderlyingType>(*this) / 1000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Centimeters>::operator Distance<EDistanceUnit::Meters>() const noexcept
{
    return Distance<EDistanceUnit::Meters>(static_cast<UnderlyingType>(*this) / 100.0F);
}

template <>
constexpr Distance<EDistanceUnit::Kilometers>::operator Distance<EDistanceUnit::Meters>() const noexcept
{
    return Distance<EDistanceUnit::Meters>(static_cast<UnderlyingType>(*this) * 1000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Kilometers>::operator Distance<EDistanceUnit::Centimeters>() const noexcept
{
    return Distance<EDistanceUnit::Centimeters>(static_cast<UnderlyingType>(*this) * 100'000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Meters>::operator Distance<EDistanceUnit::Centimeters>() const noexcept
{
    return Distance<EDistanceUnit::Centimeters>(static_cast<UnderlyingType>(*this) * 100.0F);
}

// Type literals

constexpr Centimeters operator"" _cm(RkLdouble const in_distance) noexcept
{
    return Centimeters(static_cast<Centimeters::UnderlyingType>(in_distance));
}

constexpr Centimeters operator"" _cm(RkSize const in_distance) noexcept
{
    return Centimeters(static_cast<Centimeters::UnderlyingType>(in_distance));
}

constexpr Meters operator"" _m(RkLdouble const in_distance) noexcept
{
    return Meters(static_cast<Meters::UnderlyingType>(in_distance));
}

constexpr Meters operator"" _m(RkSize const in_distance) noexcept
{
    return Meters(static_cast<Meters::UnderlyingType>(in_distance));
}

constexpr Kilometers operator"" _km(RkLdouble const in_distance) noexcept
{
    return Kilometers(static_cast<Kilometers::UnderlyingType>(in_distance));
}

constexpr Kilometers operator"" _km(RkSize const in_distance) noexcept
{
    return Kilometers(static_cast<Kilometers::UnderlyingType>(in_distance));
}

#pragma endregion