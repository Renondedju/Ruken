
#pragma once

#include <string>

#include "Debug/Logging/LogLevel.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Instances of this structure are created automatically by the Logger every time something is logged
 *        and can be created manually.
 *        It contains all the information pertinent to the event being logged.
 */
struct LogRecord
{
    ELogLevel   level       {ELogLevel::Debug};
    std::string logger_name {""};
    std::string message     {""};
};

END_RUKEN_NAMESPACE