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

#include "Debug/Logging/Handlers/FileHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

FileHandler::FileHandler(LogFormatter       const& in_formatter,
                         std::string        const& in_path,
                         std::ios::openmode const  in_mode) noexcept:
    LogHandler {in_formatter},
    m_stream   {in_path, in_mode}
{

}

#pragma endregion

#pragma region Methods

DAEvoid FileHandler::Flush()
{
    if (!m_stream.is_open())
        return;

    while (!m_records.Empty())
    {
        LogRecord record;

        m_records.Dequeue(record);

        m_stream << m_formatter.Format(record);
    }
}

#pragma endregion