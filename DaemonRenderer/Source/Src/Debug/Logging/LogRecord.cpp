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

#include <utility>


#include "Debug/Logging/LogRecord.hpp"

USING_DAEMON_NAMESPACE

LogRecord::LogRecord(String          in_logger_name,
                     ELogLevel const in_level,
                     String          in_message,
                     String          in_filename,
                     DAEint32  const in_line_number,
                     String          in_function_name) noexcept :
    logger_name     {std::move(in_logger_name)},
    level           { in_level },
    message         {std::move(in_message)},
    filename        {std::move(in_filename)},
    line_number     { in_line_number },
    function_name   {std::move(in_function_name)}
{

}

DAEbool LogRecord::operator==(LogRecord const& in_other) const noexcept
{
    return logger_name   == in_other.logger_name &&
           level         == in_other.level       &&
           message       == in_other.message     &&
           filename      == in_other.filename    &&
           line_number   == in_other.line_number &&
           function_name == in_other.function_name;
}

DAEbool LogRecord::operator!=(LogRecord const& in_other) const noexcept
{
    return logger_name   != in_other.logger_name ||
           level         != in_other.level       ||
           message       != in_other.message     ||
           filename      != in_other.filename    ||
           line_number   != in_other.line_number ||
           function_name != in_other.function_name;
}