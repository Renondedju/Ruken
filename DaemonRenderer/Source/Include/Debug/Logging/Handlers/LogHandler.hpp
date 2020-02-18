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

#include "Containers/ForwardList.hpp"

#include "../Filters/LogFilter.hpp"
#include "../Formatters/LogFormatter.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Handler objects are responsible for dispatching the appropriate log messages (based on the log messagesÅf severity) to the handlerÅfs specified destination.
 *
 * \note Logger objects can add zero or more handler objects to themselves with an AddHandler() method.
 */
class LogHandler
{
    protected:

        #pragma region Members

        /**
         * 
         */
        ELogLevel m_level;

        /**
         * 
         */
        LogFormatter const* m_formatter;

        /**
         * 
         */
        ForwardList<LogFilter const*> m_filters;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Does whatever it takes to actually log the specified logging record.
         *
         * \param in_record The record to emit.
         *
         * \note This version is intended to be implemented by subclasses.
         */
        virtual DAEvoid Emit(LogRecord const& in_record) = 0;

        #pragma endregion

    public:

        #pragma region Contructors and Destructor

        explicit LogHandler(ELogLevel in_level = ELogLevel::NotSet) noexcept;

        LogHandler(LogHandler const&    in_copy) = delete;
        LogHandler(LogHandler&&         in_move) = delete;

        virtual ~LogHandler() noexcept;

        #pragma endregion

        #pragma region Operators

        LogHandler& operator=(LogHandler const& in_other) = delete;
        LogHandler& operator=(LogHandler&&      in_other) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Initializes a thread lock which can be used to serialize access to underlying I/O functionality which may not be thread-safe.
         */
        DAEvoid CreateLock();

        /**
         * \brief Acquires the thread lock created with CreateLock().
         */
        DAEvoid Acquire();

        /**
         * \brief Releases the thread lock acquired with Acquire().
         */
        DAEvoid Release();

        /**
         * \brief Sets the threshold for this handler to the specified level.
         *
         * Logging messages which are less severe than this level will be ignored.
         *
         * \param in_level The desired log level.
         *
         * \note When a handler is created, the level is set to "NotSet" (which causes all messages to be processed).
         */
        DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \brief Sets the formatter for this handler to the specified one.
         *
         * \param in_formatter The desired formatter.
         */
        DAEvoid SetFormatter(LogFormatter const* in_formatter) noexcept;

        /**
         * \brief Adds the specified filter filter to this handler.
         *
         * \param in_filter The filter to add.
         */
        DAEvoid AddFilter(LogFilter const* in_filter);

        /**
         * \brief Removes the specified filter filter from this handler.
         *
         * \param in_filter The filter to remove.
         */
        DAEvoid RemoveFilter(LogFilter const* in_filter);

        /**
         * \brief Applies this handlerÅfs filters to the record.
         *
         * The filters are consulted in turn, until one of them returns a false value.
         * If none of them return a false value, the record will be emitted.
         * If one returns a false value, the handler will not emit the record.
         *
         * \return True if the record is to be processed, else False.
         */
        DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Conditionally emits the specified logging record, depending on filters which may have been added to the handler.
         *
         * \param in_record The record to handle.
         *
         * \note Wraps the actual emission of the record with acquisition/release of the I/O thread lock.
         */
        DAEvoid Handle(LogRecord const& in_record) noexcept;

        /**
         * \brief This method should be called from handlers when an exception is encountered during an Emit() call.
         *
         * \param in_record The record to handle.
         */
        DAEvoid HandleError(LogRecord const& in_record) const noexcept;

        /**
         * \brief Does formatting for a record - if a formatter is set, use it. Otherwise, use the default formatter for the module.
         *
         * \param in_record The record to format.
         *
         * \return The resulting string.
         */
        String Format(LogRecord const& in_record) const noexcept;

        /**
         * \brief Ensures all logging output has been flushed.
         *
         * \note This version does nothing and is intended to be implemented by subclasses.
         */
        virtual DAEvoid Flush();

        /**
         * \brief Tidies up any resources used by the handler.
         *
         * This version does no output but removes the handler from an internal list of handlers which is closed when Shutdown() is called.
         *
         * \note Subclasses should ensure that this gets called from overridden Close() methods.
         */
        virtual DAEvoid Close();

        #pragma endregion
};

END_DAEMON_NAMESPACE