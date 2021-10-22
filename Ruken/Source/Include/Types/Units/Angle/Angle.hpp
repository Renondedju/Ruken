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

#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

#include "Maths/Constants.hpp"
#include "Types/Units/Angle/EAngleUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed angle type
 * \tparam TUnitType Unit type of the angle, must be either Degrees or Radians
 */
template<EAngleUnit TUnitType>
struct Angle final: StrongType<RkFloat, Angle<TUnitType>>,
                    Arithmetic<Angle<TUnitType>>,
                    Comparison<Angle<TUnitType>>,
                    Stream    <Angle<TUnitType>>
{
    #pragma region Constructors

    using StrongType<RkFloat, Angle<TUnitType>>::StrongType;

    constexpr Angle()                     = default;
    constexpr Angle(Angle const& in_copy) = default;
    constexpr Angle(Angle&&      in_move) = default;
             ~Angle()                     = default;

    #pragma endregion

    #pragma region Operators

    constexpr Angle& operator=(Angle const& in_copy) = default;
    constexpr Angle& operator=(Angle&&		in_move) = default;

    /**
     * \brief Degree angle conversion
     */
    [[nodiscard]]
    constexpr operator Angle<EAngleUnit::Degree>() const noexcept;

    /**
     * \brief Radian angle conversion
     */
    [[nodiscard]]
    constexpr operator Angle<EAngleUnit::Radian>() const noexcept;

    #pragma endregion
};

// Type aliases

using Degrees = Angle<EAngleUnit::Degree>;
using Radians = Angle<EAngleUnit::Radian>;

// Suffixes

template<>
struct StrongTypeSuffix<Degrees>
{
    static constexpr const RkChar* suffix = " deg";
};

template<>
struct StrongTypeSuffix<Radians>
{
    static constexpr const RkChar* suffix = " rad";
};

// Type literals

constexpr Degrees operator"" _deg(RkLdouble in_angle) noexcept;
constexpr Degrees operator"" _deg(RkSize    in_angle) noexcept;
constexpr Radians operator"" _rad(RkLdouble in_angle) noexcept;
constexpr Radians operator"" _rad(RkSize    in_angle) noexcept;

#include "Types/Units/Angle/Angle.inl"


// Constants

template <>
struct Constants<Radians>
{
    static constexpr Radians pi   = 3.14159265358979323846_rad;
    static constexpr Radians pi_2 = 1.57079632679489661923_rad; // pi / 2
    static constexpr Radians pi_4 = 0.78539816339744830962_rad; // pi / 4
};

END_RUKEN_NAMESPACE