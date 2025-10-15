#include "Core/Debug/Trace.hpp"
#include "Core/Types/FundamentalTypes.hpp"

#include <tracy/Tracy.hpp>
#include <source_location>

USING_RUKEN_NAMESPACE

___tracy_source_location_data* TracyUtilities::GetOrInsertSourceLocationData(
    std::source_location const& in_source_location,
    const    char*              in_name,
    uint32_t const              in_color) noexcept
{
#ifdef TRACY_ENABLE
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
#else
    return nullptr;
#endif
}

TracyCZoneCtx TracyUtilities::TracyZone(___tracy_source_location_data* in_source_data, bool in_active) noexcept
{
    TracyCZoneCtx ctx {};

#ifdef TRACY_ENABLE
    #if defined TRACY_HAS_CALLSTACK && defined TRACY_CALLSTACK
        ctx = ___tracy_emit_zone_begin_callstack(in_source_data, TRACY_CALLSTACK, in_active);
    #else
        ctx = ___tracy_emit_zone_begin(in_source_data, in_active);
    #endif
#endif

    return ctx;
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location in_source_location, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location in_source_location, std::string_view const in_name, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location, in_name.data()),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location in_source_location, uint32_t const in_color, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location, nullptr, in_color),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location in_source_location,
    std::string_view const in_name, uint32_t const in_color, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.
        GetOrInsertSourceLocationData(in_source_location, in_name.data(), in_color), in_active);
}

RkVoid TracyUtilities::TracyZoneEnd(TracyCZoneCtx const& out_context) noexcept
{
    TracyCZoneEnd(out_context);
}

#ifdef TRACY_ENABLE

    void* operator new (RkSize const in_count)
    {
        auto const ptr {malloc(in_count)};
        TracyAlloc(ptr, in_count);
        return ptr;
    }

    void operator delete(void* in_ptr) noexcept
    {
        TracyFree(in_ptr);
        free     (in_ptr);
    }

    void operator delete(void* in_ptr, std::size_t) noexcept
    {
        TracyFree(in_ptr);
        free     (in_ptr);
    }

#endif