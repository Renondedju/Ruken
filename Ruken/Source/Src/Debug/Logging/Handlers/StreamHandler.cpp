
#include "Debug/Logging/Handlers/StreamHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor

StreamHandler::StreamHandler(LogFormatter const& in_formatter, std::ostream const& in_stream) noexcept:
    LogHandler {in_formatter},
    m_stream   {in_stream.rdbuf()}
{

}

StreamHandler::~StreamHandler()
{
    Flush();
}

#pragma endregion

#pragma region Methods

RkVoid StreamHandler::Flush()
{
    while (!m_records.Empty())
    {
        LogRecord record;

        m_records.Dequeue(record);

        m_stream << m_formatter.Format(record);
    }
}

#pragma endregion