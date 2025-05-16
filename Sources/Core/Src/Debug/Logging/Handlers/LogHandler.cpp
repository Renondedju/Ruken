
#include "Debug/Logging/Handlers/LogHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

LogHandler::LogHandler(LogFormatter const& in_formatter) noexcept:
    m_formatter {in_formatter}
{}

#pragma endregion

#pragma region Methods

RkVoid LogHandler::Handle(LogRecord&& in_record) noexcept
{
    m_records.Enqueue(std::forward<LogRecord>(in_record));
}

#pragma endregion