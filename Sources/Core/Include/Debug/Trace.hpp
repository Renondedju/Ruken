#pragma once

#include "Debug/SourceLocation.hpp"

#include <Tracy/TracyC.h>
#include <unordered_map>
#include <shared_mutex>

/// A collection of functions and members used by the executive system to properly integrate the tracy profiler
struct TracyUtilities
{
	___tracy_source_location_data* GetOrInsertSourceLocationData(std::source_location const& in_source_location, const char* in_name = nullptr, uint32_t const in_color = 0) noexcept
	{
		{
			std::shared_lock shared_lock(m_source_location_mutex);

			if (m_source_location_registery.contains(in_source_location))
				return &m_source_location_registery.at(in_source_location);
		}

		std::unique_lock unique_lock(m_source_location_mutex);

		m_source_location_registery.try_emplace(in_source_location, ___tracy_source_location_data {
			in_name, in_source_location.function_name(), in_source_location.file_name(), in_source_location.line(), in_color
		});

		return &m_source_location_registery.at(in_source_location);
	}

	private:

		std::shared_mutex												        m_source_location_mutex     {};
		std::unordered_map<std::source_location, ___tracy_source_location_data> m_source_location_registery {};
};

static inline TracyUtilities s_tracy_utilities {};

#if defined TRACY_HAS_CALLSTACK && defined TRACY_CALLSTACK
#  define TracySCZone(   ctx, source_location, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line(), 0 }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#  define TracySCZoneN(  ctx, source_location, name, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, 0 }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK active );
#  define TracySCZoneC(  ctx, source_location, color, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#  define TracySCZoneNC( ctx, source_location, name, color, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#else
#  define TracySCZone(   ctx, source_location, active ) auto source_location_data {s_tracy_utilities.GetOrInsertSourceLocationData(source_location)}; ctx = ___tracy_emit_zone_begin(source_location_data, active);
#  define TracySCZoneN(  ctx, source_location, name, active ) auto source_location_data {s_tracy_utilities.GetOrInsertSourceLocationData(source_location, name)}; ctx = ___tracy_emit_zone_begin(source_location_data, active);
#  define TracySCZoneC(  ctx, source_location, color, active ) auto source_location_data {s_tracy_utilities.GetOrInsertSourceLocationData(source_location, nullptr, color)}; ctx = ___tracy_emit_zone_begin(source_location_data, active);
#  define TracySCZoneNC( ctx, source_location, name, color, active ) auto source_location_data {s_tracy_utilities.GetOrInsertSourceLocationData(source_location, name, color)}; ctx = ___tracy_emit_zone_begin(source_location_data, active);
#endif
