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

#include <initializer_list>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Returns the minimum of the 2 passed values
 *
 * \param in_lhs Left hand side operand
 * \param in_rhs Right hand side operand
 *
 * \return Minimum value
 */
[[nodiscard]] RkFloat Minimum(RkFloat in_lhs, RkFloat in_rhs) noexcept;

/**
 * \brief Returns the minimum of the 2 passed values except if one of the 2 is zero (then the other one is returned instead)
 *
 * \param in_lhs Left hand side operand
 * \param in_rhs Right hand side operand
 *
 * \return Minimum except 0 value
 */
[[nodiscard]] RkFloat MinExceptZero(RkFloat in_lhs, RkFloat in_rhs) noexcept;

/**
 * \brief Returns the minimum of the passed values except if one of them is zero (then the other one is returned instead)
 * \param in_list Operands
 * \return Minimum except 0 value
 */
[[nodiscard]] RkFloat MinExceptZero(std::initializer_list<RkFloat> in_list) noexcept;

/**
 * \brief Returns the maximum of the 2 passed values
 *
 * \param in_lhs Left hand side operand
 * \param in_rhs Right hand side operand
 *
 * \return Maximum value
 */
[[nodiscard]] RkFloat Maximum(RkFloat in_lhs, RkFloat in_rhs) noexcept;

END_RUKEN_NAMESPACE