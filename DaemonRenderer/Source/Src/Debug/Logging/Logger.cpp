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

#pragma region      Public Constructor

Logger::Logger(String const& in_name) noexcept :
    m_name { in_name }
{
    
}

#pragma endregion   Public Constructor

#pragma region      Public Methods

ELogLevel   Logger::GetLevel        () const noexcept
{
	return m_level;
}

DAEvoid     Logger::SetLevel        (ELogLevel in_level) noexcept
{
	m_level = in_level;
}

DAEbool     Logger::IsEnabledFor    (ELogLevel in_level) const noexcept
{
	return m_level <= in_level;
}

DAEvoid     Logger::Debug           () noexcept
{
	if (m_level <= ELogLevel::Debug)
	{
	    
	}
}

DAEvoid     Logger::Info            () noexcept
{
	if (m_level <= ELogLevel::Info)
	{

	}
}

DAEvoid     Logger::Warning         () noexcept
{
	if (m_level <= ELogLevel::Warning)
	{

	}
}

DAEvoid     Logger::Error           () noexcept
{
	if (m_level <= ELogLevel::Error)
	{

	}
}

DAEvoid     Logger::Fatal           () noexcept
{

}

DAEvoid     Logger::AddFilter       (LogFilter const& in_filter) noexcept
{
	m_filters.push_front(in_filter);
}

DAEvoid     Logger::RemoveFilter    (LogFilter const& in_filter) noexcept
{
	m_filters.remove(in_filter);
}

DAEbool     Logger::Filter          (LogRecord const& in_record) noexcept
{
	return false;
}

DAEvoid     Logger::AddHandler      (Handler const& in_handler) noexcept
{
	m_handlers.push_front(in_handler);
}

DAEvoid     Logger::RemoveHandler   (Handler const& in_handler) noexcept
{
	m_handlers.remove(in_handler);
}

DAEbool     Logger::Handle          (LogRecord const& in_record) noexcept
{
	return false;
}

DAEbool     Logger::HasFilters      () const noexcept
{
	return !m_filters.empty();
}

DAEbool     Logger::HasHandlers     () const noexcept
{
	return !m_handlers.empty();
}

#pragma endregion   Public Methods