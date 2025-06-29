#pragma once

#include "JobSystem/Blocking/Synchronized.hpp"
#include "Debug/Logging/Formatters/LogFormatter.hpp"

#include <queue>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief This is the base class responsible for dispatching the appropriate log messages
 *        (based on the log messages�' severity) to the handler�'s specified destination.
 * @note  Logger objects can add zero or more handler objects to themselves with an "AddHandler" method.
 */
class LogHandler
{
    protected:

        #pragma region Members

        LogFormatter const&                 m_formatter {};
        Synchronized<std::queue<LogRecord>> m_records   {};

        #pragma endregion

    public:

        #pragma region Lifetime

        explicit LogHandler(LogFormatter const& in_formatter) noexcept;
        LogHandler           (LogHandler const&) = delete;
        LogHandler           (LogHandler&&     ) = delete;
        LogHandler& operator=(LogHandler const&) = delete;
        LogHandler& operator=(LogHandler&&     ) = delete;
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