
#include <iostream>

#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

ConsoleHandler::ConsoleHandler() noexcept:
    StreamHandler {m_console_formatter, std::cout}
{
    
}

#pragma endregion