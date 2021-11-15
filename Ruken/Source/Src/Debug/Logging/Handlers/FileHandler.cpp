
#include "Debug/Logging/Handlers/FileHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

FileHandler::FileHandler(LogFormatter       const& in_formatter,
                         std::string        const& in_path,
                         std::ios::openmode const  in_mode) noexcept:
    LogHandler {in_formatter},
    m_stream   {in_path, in_mode}
{

}

#pragma endregion

#pragma region Methods

RkVoid FileHandler::Flush()
{
    if (!m_stream.is_open())
        return;

    while (!m_records.Empty())
    {
        LogRecord record;

        m_records.Dequeue(record);

        m_stream << m_formatter.Format(record);
    }
}

#pragma endregion