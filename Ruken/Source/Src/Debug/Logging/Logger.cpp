/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Logger::Logger(ServiceProvider&        in_service_provider,
               std::string_view const  in_name,
               ELogLevel        const  in_level, 
               Logger           const* in_parent) noexcept:
    Service  {in_service_provider},
    m_name   {in_name},
    m_level  {in_level},
    m_parent {in_parent}
{ }

#pragma endregion

#pragma region Methods

RkVoid Logger::ForceHandle(LogRecord const& in_record) const noexcept
{
    if (propagate && m_parent)
        m_parent->ForceHandle(in_record);

    for (auto* handler : m_handlers)
        handler->Handle(in_record);
}

RkVoid Logger::SetLevel(ELogLevel const in_level) noexcept
{
	m_level = in_level;
}

RkBool Logger::IsEnabledFor(ELogLevel const in_level) const noexcept
{
	return in_level >= m_level;
}

Logger* Logger::AddChild(std::string_view const in_name) noexcept
{
    return &m_children.emplace_front(m_service_provider, in_name, m_level, this);
}

RkVoid Logger::Log(ELogLevel const in_level, std::string_view const in_message) const noexcept
{
    if (!IsEnabledFor(in_level))
        return;

    Handle(LogRecord {in_level, m_name, std::string(in_message)});
}

RkVoid Logger::Debug(std::string_view const in_message) const noexcept
{
    Log(ELogLevel::Debug, in_message);
}

RkVoid Logger::Info(std::string_view const in_message) const noexcept
{
    Log(ELogLevel::Info, in_message);
}

RkVoid Logger::Warning(std::string_view const in_message) const noexcept
{
    Log(ELogLevel::Warning, in_message);
}

RkVoid Logger::Error(std::string_view const in_message) const noexcept
{
    Log(ELogLevel::Error, in_message);
}

RkVoid Logger::Fatal(std::string_view const in_message) const noexcept
{
    Log(ELogLevel::Fatal, in_message);
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

RkBool Logger::Filter(LogRecord const& in_record) const noexcept
{
    for (auto const* filter : m_filters)
    {
        if (!filter->Filter(in_record))
            return false;
    }

    return true;
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

RkVoid Logger::Handle(LogRecord const& in_record) const noexcept
{
    if (Filter(in_record))
        ForceHandle(in_record);
}

RkBool Logger::HasHandlers() const noexcept
{
    if (propagate && m_parent)
        return !m_handlers.empty() || m_parent->HasHandlers();

    return !m_handlers.empty();
}

#pragma endregion