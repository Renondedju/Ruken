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

#include "ELogLevel.hpp"

#include "Containers/String.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Instances of this class are created automatically by the Logger every time something is logged and be created manually.
 *
 * It contains all the information pertinent to the event being logged.
 */
class LogRecord
{
    private:

        #pragma region Members

		String      m_logger_name;
		ELogLevel   m_level;
		String      m_path;
		DAEuint32   m_line_number;
		String      m_message;
		String      m_function_name;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        LogRecord (String    const& in_logger_name,
                   ELogLevel const  in_level,
                   String    const& in_path,
                   DAEuint32 const  in_line_number,
                   String    const& in_message,
                   String    const& in_function_name = "") noexcept;

		LogRecord(LogRecord const&  in_copy) noexcept = default;
		LogRecord(LogRecord&&       in_move) noexcept = default;

		~LogRecord() = default;

        #pragma endregion

        #pragma region Operators

		LogRecord& operator=(LogRecord const&   in_copy) noexcept = default;
		LogRecord& operator=(LogRecord&&        in_move) noexcept = default;

		DAEbool operator==(LogRecord const& in_other) const noexcept;
		DAEbool operator!=(LogRecord const& in_other) const noexcept;

        #pragma endregion

        #pragma region Methods

        String GetMessage() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE