#pragma once

#include "Types/FundamentalTypes.hpp"

#include <algorithm>
#include <new>

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable:4324) // 'ruken::Concurrency': structure was padded due to alignment specifier
#endif

BEGIN_RUKEN_NAMESPACE

/**
 * This structure packs multiple values in a single int to be used in an atomic variable.
 * Doing it this way allows the usage of fetch_add and fetch_sub instead of just compare_exchange_*.
 */
struct alignas(std::hardware_destructive_interference_size) Concurrency
{
	struct Fields
	{
		RkSize maximum   : 16 {255};
		RkSize current   : 16 {0};
		RkSize requested : 16 {0};
		RkSize optimal   : 16 {0};
	};

	union
	{
		Fields fields {};
		RkSize packed_value;
	};

	#pragma region Operators

	operator	   RkSize&()	   noexcept { return packed_value; }
	operator const RkSize&() const noexcept { return packed_value; }

	Concurrency operator+(Concurrency const& in_other) const { return Concurrency { .packed_value = packed_value + in_other.packed_value }; }
	Concurrency operator-(Concurrency const& in_other) const { return Concurrency { .packed_value = packed_value - in_other.packed_value }; }

	#pragma endregion

	RkInt64 ComputeRequest(RkInt64 const in_offset) const noexcept
	{
		return std::min(fields.optimal, fields.maximum) - (fields.current + in_offset);
	}
};

static constexpr Concurrency s_one_optimal_concurrency   { .fields = { .maximum = 0, .current = 0, .requested = 0, .optimal = 1} };
static constexpr Concurrency s_one_current_concurrency   { .fields = { .maximum = 0, .current = 1, .requested = 0, .optimal = 0} };
static constexpr Concurrency s_one_maximum_concurrency   { .fields = { .maximum = 1, .current = 0, .requested = 0, .optimal = 0} };
static constexpr Concurrency s_one_requested_concurrency { .fields = { .maximum = 0, .current = 0, .requested = 1, .optimal = 0} };

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning(pop) // 'ruken::Concurrency': structure was padded due to alignment specifier
#endif

static_assert(sizeof(Concurrency) <= std::hardware_constructive_interference_size);

END_RUKEN_NAMESPACE
