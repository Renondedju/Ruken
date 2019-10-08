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

#pragma once

#include "Config.hpp"

#include "Containers/String.hpp"

#include "Debug/Logging/ELogLevel.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Instances of this class are created automatically by the Logger every time something is logged.
 */
class LogRecord
{
    private:

        #pragma region      Members

		ELogLevel   m_level;
		String      m_logger_name;
		String      m_message;
		String      m_filename;
		String      m_function_name;
		DAEint32    m_line_number;

        #pragma endregion   Members

    public:

        #pragma region      Public Constructors and Destructor

		LogRecord(ELogLevel     in_level,
			      String const& in_logger_name,
			      String const& in_message,
			      String const& in_filename,
			      String const& in_function_name,
			      DAEint32      in_line_number) noexcept;

		LogRecord(LogRecord const&  in_copy) noexcept = default;
		LogRecord(LogRecord&&       in_move) noexcept = default;

		~LogRecord() = default;

        #pragma endregion   Public Constructors and Destructor

        #pragma region      Public Operators

		LogRecord& operator=(LogRecord const&   in_copy) noexcept = default;
		LogRecord& operator=(LogRecord&&        in_move) noexcept = default;

		DAEbool operator==(LogRecord const& in_other) const noexcept;
		DAEbool operator!=(LogRecord const& in_other) const noexcept;

        #pragma endregion   Public Operators
};

END_DAEMON_NAMESPACE