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

#include "Containers/Queue.hpp"

#include "Threading/Synchronized.hpp"

#include "Debug/Logging/LogRecord.hpp"
#include "Debug/Logging/LogFormatter.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class sends the log records (created by loggers) to the appropriate destination.
 */
class __declspec(novtable) Handler
{
    protected:

		ELogLevel                       m_level;
		LogFormatter&                   m_formatter;
		Synchronized<Queue<LogRecord>>  m_log_records;

		using LogRecordsReadAccess  = decltype(m_log_records)::ReadAccess;
		using LogRecordsWriteAccess = decltype(m_log_records)::WriteAccess;

    public:

        #pragma region      Constructors and Destructor

		Handler(LogFormatter&   in_formatter)   noexcept;
		Handler(Handler const&  in_copy)        noexcept = delete;
		Handler(Handler&&       in_move)        noexcept = delete;

        virtual ~Handler() = default;

        #pragma endregion   Constructors and Destructor

        #pragma region      Local Operators

		Handler& operator=(Handler const& in_copy) noexcept = delete;
		Handler& operator=(Handler&&      in_move) noexcept = delete;

		virtual DAEbool operator==(Handler const& in_other) const = 0;
		virtual DAEbool operator!=(Handler const& in_other) const = 0;

        #pragma endregion   Local Operators

        #pragma region      Local Methods

		DAEvoid SetLevel    (ELogLevel      in_level)       noexcept;
		DAEvoid SetFormatter(LogFormatter&  in_formatter)   const noexcept;

		virtual DAEvoid Flush   ()                                = 0;
		virtual DAEvoid Close   ()                                = 0;
		virtual DAEvoid Handle  (LogRecord&& in_log_record)       = 0;
		virtual DAEvoid Emit    (LogRecord&& in_log_record) const = 0;

        #pragma endregion   Local Methods
};

END_DAEMON_NAMESPACE