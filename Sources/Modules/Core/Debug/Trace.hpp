#pragma once

#include "Build/BuildInfo.hpp"
#include "Core/Build/Namespace.hpp"
#include "Core/Debug/SourceLocation.hpp"
#include "Core/Types/FundamentalTypes.hpp"
#include "Core/Debug/TraceAllocations.hpp"

#include <tracy/TracyC.h>
#include <string_view>

#ifdef RUKEN_TRACE_BUILD

#include <unordered_map>
#include <shared_mutex>

#else
	using ___tracy_source_location_data = void*;
#endif

BEGIN_RUKEN_NAMESPACE

/// A collection of functions and members used by the executive system to properly integrate the tracy profiler
struct TracyUtilities
{
	___tracy_source_location_data* GetOrInsertSourceLocationData(std::source_location const& in_source_location,
		const char* in_name = nullptr, uint32_t in_color = 0) noexcept;

	/**
	 * Starts a tracy zone.
	 * @param in_source_location Source location of the start of the zone
	 * @param in_name Name of the zone
	 * @param in_color Color of the zone
	 * @param in_active True if the zone is active, false otherwise
	 * @returns Zone context
	 */
	static TracyCZoneCtx TracyZone(std::source_location const& in_source_location, std::string_view in_name , uint32_t in_color, bool in_active) noexcept;
	static TracyCZoneCtx TracyZone(std::source_location const& in_source_location, std::string_view in_name ,                    bool in_active) noexcept;
	static TracyCZoneCtx TracyZone(std::source_location const& in_source_location, uint32_t		    in_color,                    bool in_active) noexcept;
	static TracyCZoneCtx TracyZone(std::source_location const& in_source_location,							                     bool in_active) noexcept;

	static TracyCZoneCtx TracyZone(___tracy_source_location_data* in_source_data, bool in_active) noexcept;

	/**
	 * Ends a tracy zone
	 * @param out_context Zone context
	 */
	static RkVoid TracyZoneEnd(TracyCZoneCtx const& out_context) noexcept;

	private:

		#ifdef RUKEN_TRACE_BUILD

		std::unordered_map<
			std::source_location, ___tracy_source_location_data,
			std::hash    <std::source_location>,
			std::equal_to<std::source_location>,
			UntrackedAllocator<std::pair<const std::source_location, ___tracy_source_location_data>
		>>				  m_source_location_registry {};
		std::shared_mutex m_source_location_mutex    {};

		#endif
};


/*
 * This value is leaked intentionally. This pointer stores a registry used by tracy for coroutine source location data.
 * Unfortunately sometimes this value is destroyed before tracy and triggers a read after free error (static initialization order fiasco).
 *
 * Transient zones could have been used instead at the cost of a new allocation for each coroutine resume operation,
 * which I don't want to pay for. Additionally, to make sure this leak won't show up in tracy itself,
 * a special new overload is used.
 */
static inline TracyUtilities* s_tracy_utilities {new (TracyNoAllocationTracking{}) TracyUtilities()};

#ifdef TRACY_ENABLE
	#define TRACY_BEGIN_ZONE(in_zone, ...) in_zone = TracyUtilities::TracyZone(__VA_ARGS__)
	#define TRACY_END_ZONE(in_zone)					 TracyUtilities::TracyZoneEnd(in_zone)
#else
	#define TRACY_BEGIN_ZONE(in_zone, ...)
	#define TRACY_END_ZONE(in_zone)
#endif

END_RUKEN_NAMESPACE
