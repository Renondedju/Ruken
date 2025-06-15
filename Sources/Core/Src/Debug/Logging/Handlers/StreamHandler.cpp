
#include "Debug/Logging/Handlers/StreamHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor

StreamHandler::StreamHandler(LogFormatter const& in_formatter, std::ostream const& in_stream) noexcept:
    LogHandler {in_formatter},
    m_stream   {in_stream.rdbuf()}
{

}

#pragma endregion

#pragma region Methods

RkVoid StreamHandler::Handle(LogRecord&& in_record) noexcept
{
    m_stream << m_formatter.Format(in_record) << std::flush;
}

#pragma endregion