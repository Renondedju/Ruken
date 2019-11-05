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

#include "Threading/SynchronizedAccess.hpp"

#include "Debug/Handlers/StreamHandler.hpp"

USING_DAEMON_NAMESPACE

#pragma region      Constructor

StreamHandler::StreamHandler(LogFormatter& in_formatter) noexcept : Handler(in_formatter)
{
    
}

#pragma endregion   Constructor

#pragma region      Local Methods

DAEvoid StreamHandler::Flush() noexcept
{
    
}

DAEvoid StreamHandler::Close() noexcept
{
    
}

DAEvoid StreamHandler::Handle(LogRecord&& in_log_record) noexcept
{
	LogRecordsWriteAccess log_records(m_log_records);

	log_records->emplace(in_log_record);
}

DAEvoid StreamHandler::Emit(LogRecord&& in_log_record) const noexcept
{
    
}

#pragma endregion   Local Method