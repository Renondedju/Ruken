
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