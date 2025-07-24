#include "Core/Debug/Logging/Handlers/LogHandler.hpp"
#include "Core/JobSystem/Blocking/SynchronizedAccess.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

LogHandler::LogHandler(LogFormatter const& in_formatter) noexcept:
    m_formatter {in_formatter}
{}

#pragma endregion

#pragma region Methods

RkVoid LogHandler::Handle(LogRecord&& in_record) noexcept
{
    auto const access {m_records.Write()};

    access->push(std::forward<LogRecord>(in_record));
}

#pragma endregion