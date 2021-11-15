
#include "Debug/Logging/Handlers/LogHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

LogHandler::LogHandler(LogFormatter const& in_formatter) noexcept:
    m_formatter {in_formatter}
{
    
}

#pragma endregion

#pragma region Methods

RkVoid LogHandler::Handle(LogRecord const& in_record) noexcept
{
    m_records.Enqueue(LogRecord(in_record));
}

#pragma endregion