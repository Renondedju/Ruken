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

#include "Handlers/FileHandler.hpp"
#include "Handlers/StreamHandler.hpp"

#include "Containers/String.hpp"
#include "Containers/ForwardList.hpp"

#include "Types/Unique.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class exposes the interface that application code directly uses.
 *
 * \note Loggers should NEVER be instantiated directly, but always through the module-level function Debug.GetLogger(name).
 */
class Logger : Unique
{
    private:

        #pragma region Members

        /**
         * If this attribute evaluates to true, events logged to this logger will be passed to the handlers of higher level loggers,
         * in addition to any handlers attached to this logger. Messages are passed directly to the ancestor loggersÅf handlers,
         * neither the level nor filters of the ancestor loggers in question are considered.
         *
         * If this attribute evaluates to false, logging messages are not passed to the handlers of ancestor loggers.
         */
        DAEbool m_propagate;

        Logger*     m_parent;
		String      m_name;
		ELogLevel   m_level;

        ForwardList<LogFilter  const*> m_filters;
        ForwardList<LogHandler const*> m_handlers;

        #pragma endregion

    public:

        #pragma region Constructor and Destructor

		explicit Logger(String in_name) noexcept;

		~Logger() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the threshold for this logger to the specified level.
         *
         * Logging messages which are less severe than "in_level" will be ignored;
         * logging messages which have severity level or higher will be emitted by whichever handler or handlers service this logger,
         * unless a handlerÅfs level has been set to a higher severity level than "in_level".
         *
         * \param in_level The desired level.
         */
        DAEvoid SetLevel(ELogLevel in_level) noexcept;

        /**
         * \brief This method checks first the module's level and then the loggerÅfs effective level as determined by GetEffectiveLevel().
         *
         * \param in_level The level to check.
         *
         * \return True if a message of the specified level would be processed by this logger, else False.
         */
        DAEbool IsEnabledFor(ELogLevel in_level) const noexcept;

        /**
         * \brief If a value other than "NotSet" has been set using SetLevel(), it is returned.
         *
         * Otherwise, the hierarchy is traversed towards the root until a value other than "NotSet" is found, and that value is returned.
         *
         * \return The effective level for this logger.
         */
        ELogLevel GetEffectiveLevel() const noexcept;

        /**
         * \brief Returns a logger which is a descendant to this logger, as determined by the suffix.
         *
         * \param in_suffix The name of the logger to look for.
         *
         * \return A pointer to the desired logger if found.
         */
        Logger const* GetChild(String const& in_suffix) const noexcept;

        /**
         * \brief Logs a message with level "Debug" on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Debug(DAEchar const* in_message) const noexcept;

        /**
         * \brief Logs a message with level "Info" on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Info(DAEchar const* in_message) const noexcept;

        /**
         * \brief Logs a message with level "Warning" on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Warning(DAEchar const* in_message) const noexcept;

        /**
         * \brief Logs a message with level "Error" on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Error(DAEchar const* in_message) const noexcept;

        /**
         * \brief Logs a message with level "Fatal" on this logger.
         *
         * \param in_message The message to log.
         */
        DAEvoid Fatal(DAEchar const* in_message) const noexcept;

        /**
         * \brief Logs a message with level ERROR on this logger.
         *
         * Exception info is added to the logging message.
         *
         * \note This method should only be called from an exception handler.
         */
        DAEvoid Exception(DAEchar const* in_message) const noexcept;

        /**
         * \brief Adds the specified filter to this logger.
         *
         * \param in_filter The filter to add.
         */
        DAEvoid AddFilter(LogFilter const* in_filter);

        /**
         * \brief Removes the specified filter from this logger.
         *
         * \param in_filter The filter to remove.
         */
        DAEvoid RemoveFilter(LogFilter const* in_filter) noexcept;

        /**
         * \brief Apply this loggerÅfs filters to the record.
         *
         * The filters are consulted in turn, until one of them returns a false value.
         * If none of them return a false value, the record will be processed (passed to handlers).
         * If one returns a false value, no further processing of the record occurs.
         *
         * \param in_record The record to filter.
         *
         * \return True if the record is to be processed, else False.
         */
        DAEbool Filter(LogRecord const& in_record) const noexcept;

        /**
         * \brief Adds the specified handler to this logger.
         *
         * \param in_handler The handler to add.
         */
        DAEvoid AddHandler(LogHandler const* in_handler);

        /**
         * \brief Removes the specified handler from this logger.
         *
         * \param in_handler The handler to remove.
         */
        DAEvoid RemoveHandler(LogHandler const* in_handler) noexcept;

        /**
         * \brief Handles a record by passing it to all handlers associated with this logger and its ancestors (until a false value of propagate is found).
         *
         * This method is used for unpickled records received from a socket, as well as those created locally.
         *
         * Logger-level filtering is applied using filter().
         *
         * \param in_record The record to filter.
         *
         * \return True if the record was handled, else False.
         */
        DAEbool Handle(LogRecord const& in_record) const noexcept;

        /**
         * \brief Checks to see if this logger has any handlers configured.
         *
         * This is done by looking for handlers in this logger and its parents in the logger hierarchy.
         *
         * The method stops searching up the hierarchy whenever a logger with the 'propagate' attribute set to false is found,
         * that will be the last logger which is checked for the existence of handlers.
         *
         * \return True if a handler was found, else False.
         */
        DAEbool HasHandlers() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE