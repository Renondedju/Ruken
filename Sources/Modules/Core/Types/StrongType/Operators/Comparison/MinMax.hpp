#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct MinMax
{
	/// @returns the minimum of 2 arguments. If equal the first is always returned.
	template <typename TStrongType>
	[[nodiscard]]
	constexpr TStrongType Min(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
	{
		using TBase = TStrongType::TUnderlying;

		return static_cast<TBase>(in_lhs) <= static_cast<TBase>(in_rhs) ? in_lhs : in_rhs;
	}

	/// @returns the maximum of 2 arguments. If equal the first is always returned.
	template <typename TStrongType>
	[[nodiscard]]
	constexpr TStrongType Max(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
	{
		using TBase = TStrongType::TUnderlying;

		return static_cast<TBase>(in_lhs) >= static_cast<TBase>(in_rhs) ? in_lhs : in_rhs;
	}
};


END_RUKEN_NAMESPACE