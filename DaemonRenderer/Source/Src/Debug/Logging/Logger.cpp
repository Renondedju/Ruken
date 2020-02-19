/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

USING_DAEMON_NAMESPACE

Logger::Logger(DAEchar   const* in_name,
               ELogLevel const  in_level,
               Logger*          in_parent,
               bool      const  in_propagate) noexcept :
    m_name      { in_name },
    m_level     { in_level },
    m_parent    { in_parent },
    propagate   { in_propagate }
{
    
}

Logger::~Logger() noexcept
{
    for (auto const& child : m_children)
    {
        delete child;
    }
}

DAEvoid Logger::ForceHandle(LogRecord const& in_record) const noexcept
{
    if (propagate && m_parent)
    {
        m_parent->ForceHandle(in_record);
    }

    for (auto const& handler : m_handlers)
    {
        if (handler)
            handler->Handle(in_record);
    }
}

DAEvoid Logger::SetLevel(ELogLevel const in_level) noexcept
{
	m_level = in_level;
}

DAEbool Logger::IsEnabledFor(ELogLevel const in_level) const noexcept
{
	return in_level >= GetEffectiveLevel();
}

ELogLevel Logger::GetEffectiveLevel() const noexcept
{
	return m_level != ELogLevel::NotSet ? m_level : m_parent ? m_parent->GetEffectiveLevel() : m_level;
}

Logger* Logger::AddChild(DAEchar const* in_name)
{
    auto const& logger = new Logger(in_name, m_level, this, propagate);

    m_children.push_front(logger);

    return logger;
}

DAEbool Logger::RemoveChild(DAEchar const* in_name)
{
    for (auto const& child : m_children)
    {
        if (child->m_name == in_name)
        {
            m_children.remove(child);

            delete child;

            return true;
        }
    }

    return false;
}

DAEbool Logger::RemoveChild(Logger const* in_logger)
{
    for (auto const& child : m_children)
    {
        if (child->m_name == in_logger->m_name)
        {
            m_children.remove(child);

            delete child;

            return true;
        }
    }

    return false;
}

Logger* Logger::GetChild(DAEchar const* in_name) const noexcept
{
    for (auto const& child : m_children)
    {
        if (child->m_name == in_name)
        {
            return child;
        }
    }

    return nullptr;
}

DAEvoid Logger::Debug(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Debug, in_message);
}

DAEvoid Logger::Info(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Info, in_message);
}

DAEvoid Logger::Warning(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Warning, in_message);
}

DAEvoid Logger::Error(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Error, in_message);
}

DAEvoid Logger::Fatal(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Fatal, in_message);
}

DAEvoid Logger::Log(ELogLevel const in_level, DAEchar const* in_message) const noexcept
{
    if (IsEnabledFor(in_level))
	{
        Handle(LogRecord(m_name.c_str(), in_level, in_message));
	}
}

DAEvoid Logger::Exception(DAEchar const* in_message) const noexcept
{
    Log(ELogLevel::Error, in_message);
}

DAEvoid Logger::AddFilter(LogFilter* in_filter)
{
    m_filters.push_front(in_filter);
}

DAEvoid Logger::RemoveFilter(LogFilter* in_filter) noexcept
{
    m_filters.remove(in_filter);
}

DAEbool Logger::Filter(LogRecord const& in_record) const noexcept
{
    for (auto const& filter : m_filters)
    {
        if (filter && !filter->Filter(in_record))
        {
            return false;
        }
    }

    return true;
}

DAEvoid Logger::AddHandler(LogHandler* in_handler)
{
	m_handlers.push_front(in_handler);
}

DAEvoid Logger::RemoveHandler(LogHandler* in_handler) noexcept
{
	m_handlers.remove(in_handler);
}

DAEvoid Logger::Handle(LogRecord const& in_record) const noexcept
{
    if (Filter(in_record))
    {
        ForceHandle(in_record);
    }
}

DAEbool Logger::HasHandlers() const noexcept
{
    if (propagate && m_parent)
    {
        return !m_handlers.empty() || m_parent->HasHandlers();
    }

    return !m_handlers.empty();
}