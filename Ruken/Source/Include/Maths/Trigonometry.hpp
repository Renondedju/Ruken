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
#include "Types/Units/Angle/Radians.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Compute the cosine of an angle given in radians
 * \param in_angle Input angle
 * \return Cosine of in_value
 */
[[nodiscard]] RkFloat Cos(Radians in_angle) noexcept;

/**
 * \brief Compute the arc cosine of an angle given in radians
 * \param in_angle Input angle
 * \return Arc cosine of in_value
 */
[[nodiscard]] RkFloat ArcCos(Radians in_angle) noexcept;

/**
 * \brief Compute the sine of an angle given in radians
 * \param in_angle Input angle
 * \return Sinus of in_value
 */
[[nodiscard]] RkFloat Sin(Radians in_angle) noexcept;

/**
 * \brief Compute the arc sine of an angle given in radians
 * \param in_angle Input angle
 * \return Sinus of in_value
 */
[[nodiscard]] RkFloat ArcSin(Radians in_angle) noexcept;

/**
 * \brief Compute the tangent of an angle given in radians
 * \param in_angle Input angle
 * \return The tangent of in_value
 */
[[nodiscard]] RkFloat Tan(Radians in_angle) noexcept;

/**
 * \brief Compute the arc tangent of an angle given in radians
 * \param in_angle Input angle
 * \return The tangent of in_value
 */
[[nodiscard]] RkFloat ArcTan(Radians in_angle) noexcept;

END_RUKEN_NAMESPACE