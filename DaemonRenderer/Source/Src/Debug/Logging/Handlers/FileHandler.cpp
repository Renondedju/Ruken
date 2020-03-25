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

#include "Debug/Logging/Handlers/FileHandler.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

FileHandler::FileHandler(LogFormatter       const* in_formatter,
                         std::string        const& in_path,
                         std::ios::openmode const  in_mode,
                         ELogLevel          const  in_level) noexcept:
    LogHandler {in_formatter, in_level},
    m_stream   {in_path, in_mode}
{}

#pragma endregion

#pragma region Methods

DAEvoid FileHandler::Emit(LogRecord const& in_record)
{
    m_stream << m_formatter->Format(in_record);
}

DAEvoid FileHandler::Flush()
{
    m_stream.flush();
}

#pragma endregion