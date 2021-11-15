
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