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

#include <forward_list>

#include "Core/Service.hpp"

#include "Debug/Logging/Filters/LogFilter.hpp"

#include "Debug/Logging/Handlers/LogHandler.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Logger objects have a threefold job.
 *        First, they expose several methods to application code so that applications can log messages at runtime.
 *        Second, logger objects determine which log messages to act upon based upon severity (the default filtering facility) or filter objects.
 *        Third, logger objects pass along relevant log messages to all interested log handlers.
 * \note  Loggers should NEVER be instantiated directly, but always through the root logger's "AddChild" method.
 */
class Logger final: public Service<Logger>
{
    private:

        #pragma region Members

        std::string   m_name   {""};
        ELogLevel     m_level  {ELogLevel::Debug};
        Logger const* m_parent {nullptr};

        std::forward_list<Logger>           m_children {};
        std::forward_list<LogHandler*>      m_handlers {};
        std::forward_list<LogFilter const*> m_filters  {};

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Handles a record by passing it to all handlers associated with this logger
         *        and its parents (until a false value of 'propagate' is found).
         * \note  No filtering is applied.
         */
        DAEvoid ForceHandle(LogRecord const& in_record) const noexcept;

        #pragma endregion

    public:

        #pragma region Members

        /**
         * \brief If this attribute evaluates to true, events logged to this logger will be passed to the handlers of higher level loggers,
         *        in addition to any handlers attached to this logger. Messages are passed directly to the parent loggersÅ' handlers,
         *        neither the level nor filters of the parent loggers in question are considered.
         */
        DAEbool propagate {true};

        #pragma endregion

        #pragma region Constructors

        using Service<Logger>::Service;

        explicit Logger(ServiceProvider& in_service_provider,
                        std::string_view in_name,
                        ELogLevel        in_level  = ELogLevel::Debug,
                        Logger const*    in_parent = nullptr) noexcept;

        Logger(Logger const& in_copy) = delete;
        Logger(Logger&&      in_move) noexcept;

        ~Logger() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the threshold for this logger to the specified level.
         *        Logging messages which are less severe than 'in_level' will be ignored;
         *        logging messages which have severity level or higher will be emitted by whichever handler or handlers service this logger.
         */
        DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \return True if a message of the specified level would be processed by this logger, else False.
         */
        [[nodiscard]]
        DAEbool IsEnabledFor(ELogLevel in_level) const noexcept;

        /**
         * \return A pointer to the newly created logger with its parent set to this logger.
         * \note   A logger is always created with its parent's parameters.
         */
        Logger* AddChild(std::string_view in_name) noexcept;

        /**
         * \brief Logs a message with the specified level on this logger.
         */
        DAEvoid Log(ELogLevel in_level, std::string_view in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Debug'.
         */
        DAEvoid Debug(std::string_view in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Info'.
         */
        DAEvoid Info(std::string_view in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Warning'.
         */
        DAEvoid Warning(std::string_view in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Error'.
         */
        DAEvoid Error(std::string_view in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Fatal'.
         */
        DAEvoid Fatal(std::string_view in_message) const noexcept;

        /**
         * \brief Adds the specified filter to this logger.
         */
        DAEvoid AddFilter(LogFilter const* in_filter) noexcept;

        /**
         * \brief Removes the specified filter from this logger.
         */
        DAEvoid RemoveFilter(LogFilter const* in_filter) noexcept;

        /**
         * \brief  The filters are consulted in turn, until one of them returns a false value.
         * \return True if the record is to be processed, else False.
         */
        [[nodiscard]]
        DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Adds the specified handler to this logger.
         */
        DAEvoid AddHandler(LogHandler* in_handler) noexcept;

        /**
         * \brief Removes the specified handler from this logger.
         */
        DAEvoid RemoveHandler(LogHandler* in_handler) noexcept;

        /**
         * \brief Handles a record by passing it to all handlers associated with this logger
         *        and its parent (until a false value of 'propagate' is found).
         * \note  Logger-level filtering is applied using "Filter".
         */
        DAEvoid Handle(LogRecord const& in_record) const noexcept;

        /**
         * \brief  Checks to see if this logger has any handlers configured.
         *         This is done by looking for handlers in this logger and its parents in the logger hierarchy.
         *         The method stops searching up the hierarchy whenever a logger with the 'propagate' attribute set to false is found,
         *         that will be the last logger which is checked for the existence of handlers.
         * \return True if a handler could be found, else False.
         */
        [[nodiscard]]
        DAEbool HasHandlers() const noexcept;

        #pragma endregion

        #pragma region Operators

        Logger& operator=(Logger const& in_copy) = delete;
        Logger& operator=(Logger&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE