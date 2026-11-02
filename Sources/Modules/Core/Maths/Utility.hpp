#pragma once

#include <algorithm>
#include <limits>
#include <cmath>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
[[nodiscard]]
constexpr RkBool RelativelyClose(TType in_lhs, TType in_rhs) noexcept
{
	if (std::isnan(in_lhs) || std::isnan(in_rhs))
		return false;

	RkFloat const epsilon {1.0e-5f * std::max(std::max(std::fabs(in_lhs), std::fabs(in_rhs)), 1.0f)};
	RkFloat const delta   {std::fabs(in_lhs - in_rhs)};

	return delta <= epsilon;
}

template <typename TType>
[[nodiscard]] TType Absolute(TType in_value) noexcept
{ return in_value < TType(0.0) ? -in_value : in_value; }

template <typename TType, typename TSignType>
[[nodiscard]] TType CopySign(TType in_value, TSignType in_sign) noexcept
{ return in_sign < TSignType(0.0) ? -in_value : in_value; }

template <typename TType>
[[nodiscard]] TType Clamp(TType in_value, TType in_min, TType in_max) noexcept
{ return in_value < in_min ? in_min : in_value > in_max ? in_max : in_value; }

template <typename TType>
[[nodiscard]] TType Clamp01(TType in_value) noexcept
{ return Clamp(in_value, TType(0.0), TType(1.0)); }

template <typename TType>
[[nodiscard]] TType Clamp11(TType in_value) noexcept
{ return Clamp(in_value, TType(-1.0), TType(1.0)); }

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

END_RUKEN_NAMESPACE