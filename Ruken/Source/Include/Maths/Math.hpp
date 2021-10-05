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

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Power function
 *
 * \param in_value Input value
 * \param in_exponent Exponent
 *
 * \return Empowered value
 */
[[nodiscard]] RkFloat Pow(RkFloat in_value, RkFloat in_exponent) noexcept;

/**
 * \brief Computes the exponential raised to the specified power
 * \param in_value Input value
 * \return Exponential value of in_value
 */
[[nodiscard]] RkFloat Exp(RkFloat in_value) noexcept;

/**
 * \brief Compute the base 2 raised to the power given
 * \param in_value Input value
 * \return Result of two raised to the power of in_value.
 */
[[nodiscard]] RkFloat Exp2(RkFloat in_value) noexcept;

/**
 * \brief Compute the modulo of in_value by in_modulo
 * \note Always returns remainder toward 0, not toward the smaller multiple of in_modulo.
 *
 * \param in_value Input value
 * \param in_modulo Modulo
 *
 * \return Remainder of in_value / in_modulo.
 */
[[nodiscard]] RkFloat Mod(RkFloat in_value, RkFloat in_modulo) noexcept;

/**
 * \brief Returns the fractional part of the given value.
 * \param in_value Input value
 * \return Fractional part of in_value
 */
[[nodiscard]] RkFloat Frac(RkFloat in_value) noexcept;

/**
 * \brief Compute the square root of the given value
 * \param in_value Input value
 * \return Square root of in_value.
 */
[[nodiscard]] RkFloat Sqrt(RkFloat in_value) noexcept;

/**
 * \brief Compute the inverse square root of the given value
 * \param in_value Input value
 * \return Inverse square root of in_value.
 */
[[nodiscard]] RkFloat InvSqrt(RkFloat in_value) noexcept;

END_RUKEN_NAMESPACE