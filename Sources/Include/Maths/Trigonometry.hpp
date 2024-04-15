
#pragma once

#include "Build/Namespace.hpp"
#include "Types/Units/Angle/Angle.hpp"

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