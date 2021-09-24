
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

        virtual ~LogHandler() = 0;

        #pragma endregion

        #pragma region Methods

        virtual RkVoid Flush() = 0;

        /**
         * \brief Adds the specified record to a thread safe queue.
         */
        RkVoid Handle(LogRecord const& in_record) noexcept;

        #pragma endregion

        #pragma region Operators

        LogHandler& operator=(LogHandler const& in_other) = delete;
        LogHandler& operator=(LogHandler&&      in_other) = delete;

        #pragma endregion
};

inline LogHandler::~LogHandler() = default;

END_RUKEN_NAMESPACE