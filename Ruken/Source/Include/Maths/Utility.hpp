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

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Absolute value function
 * \param in_value Value to get the absolute of
 * \return Absolute value
 */
[[nodiscard]] RkFloat Abs(RkFloat in_value) noexcept;

/**
 * \brief Converts a value given to the nearest greater or equal integer.
 * \param in_value Input value
 * \return Ceiled value
 */
[[nodiscard]] RkFloat Ceil(RkFloat in_value) noexcept;

/**
 * \brief Clamps the value given between two value given min and max included
 *
 * \param in_value Input value
 * \param in_min Minimum value
 * \param in_max Maximum value
 *
 * \return Clamped value
 */
[[nodiscard]] RkFloat Clamp(RkFloat in_value, RkFloat in_min, RkFloat in_max) noexcept;

/**
 * \brief Clamps the value given between 0 and 1 included
 * \param in_value Input value
 * \return Clamped value
 */
[[nodiscard]] RkFloat Clamp01(RkFloat in_value) noexcept;

/**
 * \brief Converts a value to the nearest integer. Rounds up when the fraction is .5
 * \param in_value Input value
 * \return Rounded value
 */
[[nodiscard]] RkFloat Round(RkFloat in_value) noexcept;

/**
 * \brief Converts a value given to the nearest less or equal integer.
 * \param in_value Input value
 * \return Floored value
 */
[[nodiscard]] RkFloat Floor(RkFloat in_value) noexcept;

/**
 * \brief Compute a value with the magnitude of first argument and the sign of the second.
 *
 * \param in_value Input value
 * \param in_sign Sign value
 *
 * \return Signed value
 */
[[nodiscard]] RkFloat CopySign(RkFloat in_value, RkFloat in_sign) noexcept;

END_RUKEN_NAMESPACE