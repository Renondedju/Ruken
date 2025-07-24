#include "Core/Debug/Logging/Handlers/FileHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

FileHandler::FileHandler(LogFormatter       const& in_formatter,
                         std::string        const& in_path,
                         std::ios::openmode const  in_mode) noexcept:
    LogHandler {in_formatter},
    m_stream   {in_path, in_mode}
{}

#pragma endregion

#pragma region Methods

RkVoid FileHandler::Handle(LogRecord&& in_record) noexcept
{
    if (!m_stream.is_open())
        return;

    m_stream << m_formatter.Format(std::forward<LogRecord>(in_record));
}

#pragma endregion