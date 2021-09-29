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
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"

#include "Maths/Constants.hpp"
#include "Types/Units/Angle/EAngleUnit.hpp"
#include "Types/Concepts/ArithmeticType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed angle type
 * \tparam TUnitType Unit type of the angle, must be either Degrees or Radians
 * \tparam TType Underlying type of the angle, defaults to RkFloat
 */
template<EAngleUnit TUnitType, ArithmeticType TType = RkFloat>
class Angle final: public StrongType<TType, struct AnglePhantom>,
                   public Arithmetic<Angle<TUnitType, TType>>,
                   public Comparison<Angle<TUnitType, TType>>
{
    public:

        #pragma region Constructors

        using StrongType<TType, struct AnglePhantom>::StrongType;

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
         * \tparam TOtherType New precision
         */
        template <ArithmeticType TOtherType>
        [[nodiscard]]
        explicit constexpr operator Angle<EAngleUnit::Degree, TOtherType>() const noexcept;

        /**
         * \brief Radian angle conversion
         * \tparam TOtherType New precision
         */
        template <ArithmeticType TOtherType>
        [[nodiscard]]
        explicit constexpr operator Angle<EAngleUnit::Radian, TOtherType>() const noexcept;

        #pragma endregion
};

#include "Types/Units/Angle/Angle.inl"

END_RUKEN_NAMESPACE