#include "Core/Debug/Trace.hpp"
#include "Core/Types/FundamentalTypes.hpp"

#include <tracy/Tracy.hpp>
#include <source_location>

USING_RUKEN_NAMESPACE

___tracy_source_location_data* TracyUtilities::GetOrInsertSourceLocationData(
    [[maybe_unused]] std::source_location const& in_source_location,
    [[maybe_unused]] const    char*              in_name,
    [[maybe_unused]] uint32_t const              in_color) noexcept
{
#ifdef TRACY_ENABLE
    {
        std::shared_lock shared_lock(m_source_location_mutex);

        if (m_source_location_registry.contains(in_source_location))
            return &m_source_location_registry.at(in_source_location);
    }

    std::unique_lock unique_lock(m_source_location_mutex);

    m_source_location_registry.try_emplace(in_source_location, ___tracy_source_location_data {
        in_name, in_source_location.function_name(), in_source_location.file_name(), in_source_location.line(), in_color
    });

    return &m_source_location_registry.at(in_source_location);
#else
    return nullptr;
#endif
}

TracyCZoneCtx TracyUtilities::TracyZone(
    [[maybe_unused]] ___tracy_source_location_data* in_source_data,
    [[maybe_unused]] bool                           in_active) noexcept
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

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location const& in_source_location, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location const& in_source_location,
    std::string_view const in_name, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location, in_name.data()),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location const& in_source_location,
    uint32_t const in_color, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.GetOrInsertSourceLocationData(in_source_location, nullptr, in_color),
        in_active);
}

TracyCZoneCtx TracyUtilities::TracyZone(std::source_location const& in_source_location,
    std::string_view const in_name, uint32_t const in_color, bool const in_active) noexcept
{
    return TracyZone(
        ::RUKEN_NAMESPACE::s_tracy_utilities.
        GetOrInsertSourceLocationData(in_source_location, in_name.data(), in_color), in_active);
}

RkVoid TracyUtilities::TracyZoneEnd(
    [[maybe_unused]] TracyCZoneCtx const& out_context) noexcept
{
    TracyCZoneEnd(out_context);
}

#ifdef TRACY_ENABLE

    void* operator new (RkSize const in_count)
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

        auto const ptr {std::malloc(in_count)};
        TracyAlloc(ptr, in_count);
        return ptr;
    }

    void operator delete(void* in_ptr) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

        TracyFree(in_ptr);
        std::free(in_ptr);
    }

    void operator delete(void* in_ptr, std::size_t) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

        TracyFree(in_ptr);
        std::free(in_ptr);
    }

#endif