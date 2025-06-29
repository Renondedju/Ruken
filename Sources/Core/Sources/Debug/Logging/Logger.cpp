#include "Debug/Logging/Logger.hpp"

#include <tracy/TracyC.h>

USING_RUKEN_NAMESPACE

#pragma region Constructors

Logger::Logger(ServiceProvider& in_service_provider,
               ELogLevel const  in_level) noexcept:
    Service {in_service_provider, typeid(Logger)},
    m_level {in_level}
{ }

#pragma endregion

#pragma region Methods

RkVoid Logger::SetLevel(ELogLevel const in_level) noexcept
{
	m_level = in_level;
}

RkVoid Logger::AddFilter(LogFilter const* in_filter) noexcept
{
    if (in_filter)
        m_filters.emplace_front(in_filter);
}

RkVoid Logger::RemoveFilter(LogFilter const* in_filter) noexcept
{
    m_filters.remove(in_filter);
}

RkVoid Logger::AddHandler(LogHandler* in_handler) noexcept
{
    if (in_handler)
        m_handlers.emplace_front(in_handler);
}

RkVoid Logger::RemoveHandler(LogHandler* in_handler) noexcept
{
    m_handlers.remove(in_handler);
}

RkVoid Logger::Dispatch(LogRecord const& in_record) const noexcept
{
    // Checking for level
    if (in_record.level < m_level)
        return;

    // Checking filters
    for (auto const* filter : m_filters)
        if (!filter->Filter(in_record))
            return;

    // Dispatching to handlers and parents.
    for (auto* handler : m_handlers)
        handler->Handle(LogRecord(in_record));

    if (Logger const* parent {GetParent()})
        parent->Dispatch(in_record);
}

Logger::Logger(ServiceProvider& in_service_provider,
               std::initializer_list<LogHandler*> const in_handlers,
               ELogLevel const in_level) noexcept:
    Service    {in_service_provider, typeid(Logger)},
    m_level    {in_level},
    m_handlers {in_handlers}
{}

#pragma endregion