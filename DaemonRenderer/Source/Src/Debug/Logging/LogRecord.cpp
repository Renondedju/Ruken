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

#include "Debug/Logging/LogRecord.hpp"

USING_DAEMON_NAMESPACE

#pragma region      Public Constructor

LogRecord::LogRecord(ELogLevel const    in_level,
                     String    const&   in_logger_name,
                     String    const&   in_message,
                     String    const&   in_filename,
                     String    const&   in_function_name,
                     DAEint32  const    in_line_number) noexcept :
    m_level         { in_level },
    m_logger_name   { in_logger_name },
    m_message       { in_message },
    m_filename      { in_filename },
    m_function_name { in_function_name },
    m_line_number   { in_line_number }
{
    
}

#pragma endregion   Public Constructor

#pragma region      Public Operators

DAEbool LogRecord::operator==(LogRecord const& in_other) const noexcept
{
    return m_level         == in_other.m_level         &&
		   m_logger_name   == in_other.m_logger_name   &&
		   m_message       == in_other.m_message       &&
		   m_filename      == in_other.m_filename      &&
		   m_function_name == in_other.m_function_name &&
		   m_line_number   == in_other.m_line_number;
}

DAEbool LogRecord::operator!=(LogRecord const& in_other) const noexcept
{
    return m_level         != in_other.m_level         ||
		   m_logger_name   != in_other.m_logger_name   ||
		   m_message       != in_other.m_message       ||
		   m_filename      != in_other.m_filename      ||
		   m_function_name != in_other.m_function_name ||
		   m_line_number   != in_other.m_line_number;
}

#pragma endregion   Public Operators