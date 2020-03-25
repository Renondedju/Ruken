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

#include <string>
#include <forward_list>

#include "Filters/LogFilter.hpp"

#include "Handlers/LogHandler.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class exposes the interface that application code directly uses.
 *
 * Logger objects have a threefold job.
 *
 * First, they expose several methods to application code so that applications can log messages at runtime.
 *
 * Second, logger objects determine which log messages to act upon based upon severity (the default filtering facility) or filter objects.
 *
 * Third, logger objects pass along relevant log messages to all interested log handlers.
 *
 * \note Loggers should NEVER be instantiated directly, but always through the root logger's method "AddChild()".
 */
class Logger
{
    private:

        #pragma region Members

        std::string m_name;
        ELogLevel   m_level;
        Logger*     m_parent;

        std::forward_list<Logger*>     m_children;
        std::forward_list<LogFilter*>  m_filters;
        std::forward_list<LogHandler*> m_handlers;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Logs a message with the specified level on this logger.
         *
         * \param in_level The level to log.
         * \param in_message The message to log.
         */
        DAEvoid Log(ELogLevel in_level, std::string&& in_message) const noexcept;

        /**
         * \brief Handles a record by passing it to all handlers associated with this logger and its ancestors (until a false value of 'propagate' is found).
         *
         * No filtering is applied.
         *
         * \param in_record The record to handle.
         *
         * \note This methods should only be called by "Handle()".
         */
        DAEvoid ForceHandle(LogRecord const& in_record) const noexcept;

        #pragma endregion

    public:

        #pragma region Members

        /**
         * \brief If this attribute evaluates to true, events logged to this logger will be passed to the handlers of higher level loggers,
         *        in addition to any handlers attached to this logger. Messages are passed directly to the ancestor loggersÅf handlers,
         *        neither the level nor filters of the ancestor loggers in question are considered.
         *
         * \note If this attribute evaluates to false, logging messages are not passed to the handlers of ancestor loggers.
         */
        DAEbool propagate;

        #pragma endregion

        #pragma region Constructors and Destructor

        Logger() = delete;

        explicit Logger(std::string in_name,
                        ELogLevel   in_level,
                        Logger*     in_parent,
                        bool        in_propagate = true) noexcept;

        Logger(Logger const&    in_copy) = delete;
        Logger(Logger&&         in_move) = delete;

        ~Logger() noexcept;

        #pragma endregion

        #pragma region Operators

        Logger& operator=(Logger const& in_copy) = delete;
        Logger& operator=(Logger&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the threshold for this logger to the specified level.
         *
         * Logging messages which are less severe than "in_level" will be ignored;
         * logging messages which have severity level or higher will be emitted by whichever handler or handlers service this logger,
         * unless a handlerÅfs level has been set to a higher severity level than the specified one.
         *
         * \param in_level The new level.
         */
        DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \brief This method checks the loggerÅfs effective level as determined by "GetEffectiveLevel()".
         *
         * \param in_level The level to check.
         *
         * \return True if a message of the specified level would be processed by this logger, else False.
         */
        [[nodiscard]] DAEbool IsEnabledFor(ELogLevel in_level) const noexcept;

        /**
         * \brief If a value other than 'NotSet' has been set using "SetLevel()", it is returned.
         *
         * Otherwise, the hierarchy is traversed towards the root until a value other than 'NotSet' is found, and that value is returned.
         *
         * \return The effective level of this logger.
         */
        [[nodiscard]] ELogLevel GetEffectiveLevel() const noexcept;

        /**
         * \brief Creates a new logger and sets its parent as the current one.
         *
         * \param in_name The name of the logger to create.
         *
         * \return A pointer to the newly created logger.
         *
         * \note The logger is created with this logger's parameters.
         */
        Logger* AddChild(std::string in_name);

        /**
         * \brief Deletes the logger with the specified name.
         *
         * \param in_name The name of the logger to look for.
         *
         * \return True if the logger could be removed, else False.
         */
        DAEbool RemoveChild(std::string const& in_name);

        /**
         * \brief Deletes the specified logger.
         *
         * \param in_logger The logger to look for.
         *
         * \return True if the logger could be removed, else False.
         */
        DAEbool RemoveChild(Logger const* in_logger);

        /**
         * \brief Finds a logger which is a descendant to this logger, as determined by the specified name.
         *
         * \param in_name The name of the logger to look for.
         *
         * \return A pointer to the logger if it was found, else nullptr.
         */
        Logger* GetChild(std::string const& in_name) const noexcept;

        /**
         * \brief Logs a message with level 'Debug' on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Debug(std::string in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Info' on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Info(std::string in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Warning' on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Warning(std::string in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Error' on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Error(std::string in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Fatal' on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Fatal(std::string in_message) const noexcept;

        /**
         * \brief Logs a message with level 'Error' on this logger.
         *
         * Exception info is added to the logging message.
         *
         * \param in_message The message to log.
         *
         * \note This method should only be called from an exception handler.
         */
        DAEvoid Exception(std::string in_message) const noexcept;

        /**
         * \brief Adds the specified filter to this logger.
         *
         * \param in_filter The filter to add.
         */
        DAEvoid AddFilter(LogFilter* in_filter);

        /**
         * \brief Removes the specified filter from this logger.
         *
         * \param in_filter The filter to remove.
         */
        DAEvoid RemoveFilter(LogFilter* in_filter) noexcept;

        /**
         * \brief Applies this loggerÅfs filters to the record.
         *
         * The filters are consulted in turn, until one of them returns a false value.
         * If none of them return a false value, the record will be processed (passed to handlers).
         * If one returns a false value, no further processing of the record occurs.
         *
         * \param in_record The record to filter.
         *
         * \return True if the record is to be processed, else False.
         */
        [[nodiscard]] DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Adds the specified handler to this logger.
         *
         * \param in_handler The handler to add.
         */
        DAEvoid AddHandler(LogHandler* in_handler);

        /**
         * \brief Removes the specified handler from this logger.
         *
         * \param in_handler The handler to remove.
         */
        DAEvoid RemoveHandler(LogHandler* in_handler) noexcept;

        /**
         * \brief Handles a record by passing it to all handlers associated with this logger and its ancestors (until a "false" value of propagate is found).
         *
         * Logger-level filtering is applied using "Filter()".
         *
         * \param in_record The record to handle.
         *
         * \note This method is used for unpickled records received from a socket, as well as those created locally.
         */
        DAEvoid Handle(LogRecord const& in_record) const noexcept;

        /**
         * \brief Checks to see if this logger has any handlers configured.
         *
         * This is done by looking for handlers in this logger and its parents in the logger hierarchy.
         *
         * The method stops searching up the hierarchy whenever a logger with the 'propagate' attribute set to false is found,
         * that will be the last logger which is checked for the existence of handlers.
         *
         * \return True if a handler could be found, else False.
         */
        [[nodiscard]] DAEbool HasHandlers() const noexcept;

        #pragma endregion
};

/* TODO Needs to be removed when Kernel is done TODO */

extern Logger* GRootLogger;

/* TODO Needs to be removed when Kernel is done TODO */

END_DAEMON_NAMESPACE