
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