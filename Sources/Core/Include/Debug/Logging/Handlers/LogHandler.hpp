#pragma once

#include "Build/Attributes.hpp"
#include "Threading/ThreadSafeLockQueue.hpp"
#include "Debug/Logging/Formatters/LogFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This is the base class responsible for dispatching the appropriate log messages
 *        (based on the log messages�' severity) to the handler�'s specified destination.
 * \note  Logger objects can add zero or more handler objects to themselves with an "AddHandler" method.
 */
class LogHandler
{
    protected:

        #pragma region Members

        LogFormatter const&            m_formatter {};
        ThreadSafeLockQueue<LogRecord> m_records   {};

        #pragma endregion

    public:

        #pragma region Lifetime

        explicit LogHandler(LogFormatter const& in_formatter) noexcept;

        LogHandler(LogHandler const& in_copy) = delete;
        LogHandler(LogHandler&&      in_move) = delete;
        LogHandler& operator=(LogHandler const& in_other) = delete;
        LogHandler& operator=(LogHandler&&      in_other) = delete;

        virtual ~LogHandler() = default;

        #pragma endregion

        #pragma region Methods
    
        /**
         * \brief Adds the specified record to a thread safe queue.
         */
        virtual RkVoid Handle(LogRecord&& in_record) noexcept = 0;

        #pragma endregion
};

END_RUKEN_NAMESPACE