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

#include <forward_list>

#include "Config.hpp"
#include "Debug/Logging/Filters/LogFilter.hpp"
#include "Debug/Logging/Formatters/LogFormatter.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class is responsible for dispatching the appropriate log messages
 *        (based on the log messagesÅf severity) to the handlerÅfs specified destination.
 * \note Logger objects can add zero or more handler objects to themselves with an "AddHandler()" method.
 */
class LogHandler
{
    protected:

        #pragma region Members

        ELogLevel                           m_level;
        LogFormatter const*                 m_formatter;
        std::forward_list<LogFilter const*> m_filters;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Does whatever it takes to actually log the specified logging record.
         * \param in_record The record to emit.
         * \note This version is intended to be implemented by subclasses.
         */
        virtual DAEvoid Emit(LogRecord const& in_record) = 0;

        #pragma endregion

    public:

        #pragma region Contructors and Destructor

        /**
         * \brief Initializes the handler
         * \param in_formatter Formatter to use
         * \param in_level Log level to use
         */
        explicit LogHandler(LogFormatter const* in_formatter, ELogLevel in_level = ELogLevel::NotSet) noexcept;

        LogHandler(LogHandler const& in_copy) = delete;
        LogHandler(LogHandler&&      in_move) = delete;
        virtual ~LogHandler()                 = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Ensures all logging output has been flushed.
         * \note This version does nothing and is intended to be implemented by subclasses.
         */
        virtual DAEvoid Flush() = 0;

        /**
         * \brief Sets the threshold for this handler to the specified level.
         *
         * Logging messages which are less severe than this level will be ignored.
         *
         * \param in_level The desired log level.
         * \note When a handler is created, the level is set to 'NotSet' (which causes all messages to be processed).
         */
        DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \brief Sets the formatter for this handler to the specified one.
         * \param in_formatter The new formatter.
         */
        DAEvoid SetFormatter(LogFormatter const* in_formatter) noexcept;

        /**
         * \brief Adds the specified filter to this handler.
         * \param in_filter The filter to add.
         */
        DAEvoid AddFilter(LogFilter const* in_filter);

        /**
         * \brief Removes the specified filter from this handler.
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
        [[nodiscard]]
        DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Conditionally emits the specified logging record, depending on filters which may have been added to the handler.
         * \param in_record The record to handle.
         */
        DAEvoid Handle(LogRecord const& in_record) noexcept;

        /**
         * \brief This method should be called from handlers when an exception is encountered during an "Emit()" call.
         * \param in_record The record to handle.
         */
        DAEvoid HandleError(LogRecord const& in_record) noexcept;

        /**
         * \brief Does formatting for a record - if a formatter is set, use it.
         * \param in_record The record to format.
         * \return The resulting string.
         */
        [[nodiscard]]
        std::string Format(LogRecord const& in_record) const noexcept;

        #pragma endregion

        #pragma region Operators

        LogHandler& operator=(LogHandler const& in_other) = delete;
        LogHandler& operator=(LogHandler&&      in_other) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE