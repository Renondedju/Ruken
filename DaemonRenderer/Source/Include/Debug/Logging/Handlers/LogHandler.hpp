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

#include "../LogFilter.hpp"
#include "../LogRecord.hpp"
#include "../LogFormatter.hpp"

#include "Containers/ForwardList.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Handler objects are responsible for dispatching the appropriate log messages (based on the log messagesÅf severity) to the handlerÅfs specified destination.
 *
 * Logger objects can add zero or more handler objects to themselves with an AddHandler() method.
 *
 * As an example scenario, an application may want to send all log messages to a log file,
 * all log messages of error or higher to stdout, and all messages of critical to an email address.
 * This scenario requires three individual handlers where each handler is responsible for sending messages of a specific severity to a specific location.
 */
class LogHandler
{
    protected:

        #pragma region Variables

        ELogLevel m_level;
        LogFormatter const* m_formatter;
        ForwardList<LogFilter const*> m_filters;

        #pragma endregion

        #pragma region Contructors and Destructor

        LogHandler(ELogLevel in_level = ELogLevel::NotSet) noexcept;

		LogHandler(LogHandler const&    in_copy) noexcept = default;
		LogHandler(LogHandler&&         in_move) noexcept = default;

		virtual ~LogHandler() noexcept = default;

        #pragma endregion

        #pragma region Operators

        LogHandler& operator=(LogHandler const& in_other) noexcept = default;
        LogHandler& operator=(LogHandler&&      in_other) noexcept = default;

        #pragma endregion

    public:

        #pragma region Methods

        /**
         * \brief Initializes a thread lock which can be used to serialize access to underlying I/O functionality which may not be thread-safe.
         */
        virtual DAEvoid CreateLock();

        /**
         * \brief Acquires the thread lock created with CreateLock().
         */
        virtual DAEvoid Acquire();

        /**
         * \brief Releases the thread lock acquired with Acquire().
         */
        virtual DAEvoid Release();

        /**
         * \brief Sets the threshold for this handler to the specified level.
         *
         * Logging messages which are less severe than this level will be ignored.
         *
         * When a handler is created, the level is set to "NotSet" (which causes all messages to be processed).
         *
         * \param in_level The desired level.
         */
        virtual DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \brief Sets the "Formatter" for this handler to the specified one.
         *
         * \param in_formatter The desired formatter.
         */
        virtual DAEvoid SetFormatter(LogFormatter const* in_formatter) noexcept;

        /**
         * \brief Adds the specified filter filter to this handler.
         *
         * \param in_filter The filter to add.
         */
        virtual DAEvoid AddFilter(LogFilter const* in_filter);

        /**
         * \brief Removes the specified filter filter from this handler.
         *
         * \param in_filter The filter to remove.
         */
        virtual DAEvoid RemoveFilter(LogFilter const* in_filter) noexcept;

        /**
         * \brief Applies this handlerÅfs filters to the record.
         *
         * The filters are consulted in turn, until one of them returns a false value.
         * If none of them return a false value, the record will be emitted.
         * If one returns a false value, the handler will not emit the record.
         *
         * \return True if the record is to be processed, else False.
         */
        virtual DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Ensures all logging output has been flushed.
         *
         * This version does nothing and is intended to be implemented by subclasses.
         */
        virtual DAEvoid Flush();

        /**
         * \brief Tidies up any resources used by the handler.
         *
         * This version does no output but removes the handler from an internal list of handlers which is closed when Shutdown() is called.
         *
         * Subclasses should ensure that this gets called from overridden Close() methods.
         */
        virtual DAEvoid Close();

        /**
         * \brief Conditionally emits the specified logging record, depending on filters which may have been added to the handler.
         *
         * Wraps the actual emission of the record with acquisition/release of the I/O thread lock.
         *
         * \param in_record The record to handle.
         */
        virtual DAEvoid Handle(LogRecord const& in_record);

        /**
         * \brief This method should be called from handlers when an exception is encountered during an Emit() call.
         *
         * \param in_record The record to handle.
         */
        virtual DAEvoid HandleError(LogRecord const& in_record);

        /**
         * \brief Does formatting for a record - if a formatter is set, use it. Otherwise, use the default formatter for the module.
         *
         * \param in_record The record to format.
         */
        virtual DAEvoid Format(LogRecord const& in_record);

         /**
         * \brief Does whatever it takes to actually log the specified logging record.
         *
         * This version is intended to be implemented by subclasses.
         *
         * \param in_record The record to emit.
         */
        virtual DAEvoid Emit(LogRecord const& in_record) = 0;

        #pragma endregion
};

END_DAEMON_NAMESPACE