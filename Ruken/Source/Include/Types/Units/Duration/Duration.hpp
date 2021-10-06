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
#include "Types/StrongType/Operators/Stream.hpp"

#include "Types/Units/Duration/EDurationUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed angle type
 * \tparam TUnitType Unit type of the angle, must be either Degrees or Radians
 */
template<EDurationUnit TUnitType>
struct Duration final: StrongType<RkFloat, Duration<TUnitType>>,
                       Arithmetic<Duration<TUnitType>>,
                       Comparison<Duration<TUnitType>>,
                       Stream    <Duration<TUnitType>>
{
    #pragma region Constructors

    using StrongType<RkFloat, Duration<TUnitType>>::StrongType;

    constexpr Duration()                        = default;
    constexpr Duration(Duration const& in_copy) = default;
    constexpr Duration(Duration&&      in_move) = default;
             ~Duration()                        = default;

    #pragma endregion

    #pragma region Operators

    constexpr Duration& operator=(Duration const& in_copy) = default;
    constexpr Duration& operator=(Duration&&      in_move) = default;

    /**
     * \brief Minute duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Minute>() const noexcept;

    /**
     * \brief Second duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Second>() const noexcept;

    /**
     * \brief Millisecond duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Millisecond>() const noexcept;

    #pragma endregion
};

#include "Types/Units/Duration/Duration.inl"

END_RUKEN_NAMESPACE