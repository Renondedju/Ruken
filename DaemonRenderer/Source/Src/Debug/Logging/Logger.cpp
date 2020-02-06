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

Logger::Logger(String in_name) noexcept :
    m_propagate { true },
    m_parent    { nullptr },
    m_name      { std::move(in_name) },
    m_level     { ELogLevel::NotSet }
{
    
}

Logger::~Logger() noexcept
{
    
}

#pragma region Methods

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
    Logger const* logger = this;

    while (logger)
    {
        if (logger->m_level != ELogLevel::NotSet)
        {
            return logger->m_level;
        }

        logger = logger->m_parent;
    }

	return ELogLevel::NotSet;
}

Logger const* Logger::GetChild(String const& in_suffix) const noexcept
{
    return nullptr;
}

DAEvoid Logger::Debug(DAEchar const* in_message) const noexcept
{
	if (m_level <= ELogLevel::Debug)
	{

	}
}

DAEvoid Logger::Info(DAEchar const* in_message) const noexcept
{
	if (m_level <= ELogLevel::Info)
	{

	}
}

DAEvoid Logger::Warning(DAEchar const* in_message) const noexcept
{
	if (m_level <= ELogLevel::Warning)
	{

	}
}

DAEvoid Logger::Error(DAEchar const* in_message) const noexcept
{
	if (m_level <= ELogLevel::Error)
	{

	}
}

DAEvoid Logger::Fatal(DAEchar const* in_message) const noexcept
{
    if (m_level <= ELogLevel::Fatal)
    {
        
    }
}

DAEvoid Logger::Exception(DAEchar const* in_message) const noexcept
{
    if (m_level <= ELogLevel::Warning)
    {
        
    }
}

DAEvoid Logger::AddFilter(LogFilter const* in_filter)
{
    m_filters.push_front(in_filter);
}

DAEvoid Logger::RemoveFilter(LogFilter const* in_filter) noexcept
{
    m_filters.remove(in_filter);
}

DAEbool Logger::Filter(LogRecord const& in_record) const noexcept
{
    for (LogFilter const* filter : m_filters)
    {
        /** TODO Filtering the record. TODO */
        if (filter)
            return false;
    }

    return true;
}

DAEvoid Logger::AddHandler(LogHandler const* in_handler)
{
	m_handlers.push_front(in_handler);
}

DAEvoid Logger::RemoveHandler(LogHandler const* in_handler) noexcept
{
	m_handlers.remove(in_handler);
}

DAEbool Logger::Handle(LogRecord const& in_record) const noexcept
{
    if (Filter(in_record))
    {
        Logger const* logger = this;

	    while (logger)
	    {
            for (LogHandler const* handler : logger->m_handlers)
            {
                /** TODO Handling the record. TODO */
            }

            logger = logger->m_propagate ? logger->m_parent : nullptr;
	    }

        return true;
    }

	return false;
}

DAEbool Logger::HasHandlers() const noexcept
{
    Logger const* logger = this;

	while (logger)
	{
	    if (!logger->m_handlers.empty())
            return true;

        logger = logger->m_propagate ? logger->m_parent : nullptr;
	}

    return false;
}

#pragma endregion