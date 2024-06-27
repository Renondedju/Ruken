#pragma once

#include "Build/Namespace.hpp"
#include "Debug/SourceLocation.hpp"

#include <Tracy/TracyC.h>
#include <unordered_map>

#ifdef RUKEN_TRACE_BUILD

static inline std::unordered_map<std::source_location, struct ___tracy_source_location_data> s_source_location_registery {};

#endif

#if defined TRACY_HAS_CALLSTACK && defined TRACY_CALLSTACK
#  define TracySCZone(   ctx, source_location, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line(), 0 }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#  define TracySCZoneN(  ctx, source_location, name, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, 0 }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK active );
#  define TracySCZoneC(  ctx, source_location, color, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#  define TracySCZoneNC( ctx, source_location, name, color, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin_callstack( &s_source_location_registery[source_location], TRACY_CALLSTACK, active );
#else
#  define TracySCZone(   ctx, source_location, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line(), 0 }; ctx = ___tracy_emit_zone_begin( &s_source_location_registery[source_location], active );
#  define TracySCZoneN(  ctx, source_location, name, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, 0 }; ctx = ___tracy_emit_zone_begin( &s_source_location_registery[source_location], active );
#  define TracySCZoneC(  ctx, source_location, color, active ) s_source_location_registery[source_location] = { NULL, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin( &s_source_location_registery[source_location], active );
#  define TracySCZoneNC( ctx, source_location, name, color, active ) s_source_location_registery[source_location] = { name, source_location.function_name(), source_location.file_name(), (uint32_t)source_location.line, color }; ctx = ___tracy_emit_zone_begin( &s_source_location_registery[source_location], active );
#endif
