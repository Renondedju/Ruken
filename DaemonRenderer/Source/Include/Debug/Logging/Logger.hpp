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

#include "LogLevel.hpp"
#include "LogFilter.hpp"
#include "LogRecord.hpp"

#include "Containers/String.hpp"
#include "Containers/ForwardList.hpp"

#include "Debug/Handlers/Handler.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class exposes the interface that application code directly uses.
 */
class Logger
{
    private:

        #pragma region      Members

		String                  m_name;
		ELogLevel               m_level;
		ForwardList<LogFilter>  m_filters;
		ForwardList<Handler>    m_handlers;

        #pragma endregion   Members

    public:

        #pragma region      Public Constructors and Destructor

		Logger(String const&    in_name) noexcept;
		Logger(Logger const&    in_copy) noexcept = default;
		Logger(Logger&&         in_move) noexcept = default;

		~Logger() = default;

        #pragma endregion   Public Constructors and Destructor

        #pragma region      Public Operators

		Logger& operator=(Logger const& in_copy) noexcept = default;
		Logger& operator=(Logger&&      in_move) noexcept = default;

        #pragma endregion   Public Operators

        #pragma region      Public Methods

		ELogLevel   GetLevel    ()                  const noexcept;
		DAEvoid     SetLevel    (ELogLevel in_level)       noexcept;
		DAEbool     IsEnabledFor(ELogLevel in_level) const noexcept;

		DAEvoid Debug   () noexcept;
		DAEvoid Info    () noexcept;
		DAEvoid Warning () noexcept;
		DAEvoid Error   () noexcept;
		DAEvoid Fatal   () noexcept;

		DAEvoid AddFilter   (LogFilter const& in_filter) noexcept;
		DAEvoid RemoveFilter(LogFilter const& in_filter) noexcept;
		DAEbool Filter      (LogRecord const& in_record) noexcept;

		DAEvoid AddHandler      (Handler   const& in_handler)   noexcept;
		DAEvoid RemoveHandler   (Handler   const& in_handler)   noexcept;
		DAEbool Handle          (LogRecord const& in_record)    noexcept;

		DAEbool HasFilters  () const noexcept;
		DAEbool HasHandlers () const noexcept;

        #pragma endregion   Public Methods
};

END_DAEMON_NAMESPACE