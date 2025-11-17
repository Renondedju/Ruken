#pragma once

#include "Core/Debug/Logging/LogLevel.hpp"

#include <string_view>
#include <exception>
#include <string>

BEGIN_RUKEN_NAMESPACE

struct LogRecord
{
    ELogLevel          level            {ELogLevel::Debug};
    std::string_view   service_provider {"?"};
    std::string_view   service_name     {"?"};
    std::exception_ptr exception_ptr    {nullptr};
    std::string        message          {""};
};

END_RUKEN_NAMESPACE