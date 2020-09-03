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

#pragma once

#include "Threading/ThreadSafeLockQueue.hpp"

#include "Debug/Logging/Formatters/LogFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This is the base class responsible for dispatching the appropriate log messages
 *        (based on the log messagesÅ' severity) to the handlerÅ's specified destination.
 * \note  Logger objects can add zero or more handler objects to themselves with an "AddHandler" method.
 */
class __declspec(novtable) LogHandler
{
    protected:

        #pragma region Members

        LogFormatter const& m_formatter {};

        ThreadSafeLockQueue<LogRecord> m_records {};

        #pragma endregion

    public:

        #pragma region Contructors

        explicit LogHandler(LogFormatter const& in_formatter) noexcept;

        LogHandler(LogHandler const& in_copy) = delete;
        LogHandler(LogHandler&&      in_move) = delete;

        virtual ~LogHandler() = default;

        #pragma endregion

        #pragma region Methods

        virtual DAEvoid Flush() = 0;

        /**
         * \brief Adds the specified record to a thread safe queue.
         */
        DAEvoid Handle(LogRecord const& in_record) noexcept;

        #pragma endregion

        #pragma region Operators

        LogHandler& operator=(LogHandler const& in_other) = delete;
        LogHandler& operator=(LogHandler&&      in_other) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE