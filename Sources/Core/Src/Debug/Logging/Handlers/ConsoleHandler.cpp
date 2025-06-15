#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

#include <iostream>

USING_RUKEN_NAMESPACE

#pragma region Constructors

ConsoleHandler::ConsoleHandler() noexcept:
    StreamHandler {m_console_formatter, std::cout}
{}

RkVoid ConsoleHandler::Handle(LogRecord&& in_record) noexcept
{
    m_stream << m_console_formatter.Format(in_record) << std::flush;
}

#pragma endregion
