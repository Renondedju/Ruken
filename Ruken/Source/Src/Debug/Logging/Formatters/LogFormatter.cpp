
#include "Debug/Logging/Formatters/LogFormatter.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

std::string LogFormatter::ComputeLabel(LogRecord const& in_record) const noexcept
{
    switch (in_record.level)
    {
        case ELogLevel::Debug:   return "[DEBUG]";
        case ELogLevel::Info:    return "[INFO]";
        case ELogLevel::Warning: return "[WARNING]";
        case ELogLevel::Error:   return "[ERROR]";
        case ELogLevel::Fatal:   return "[FATAL]";
    }

    return "";
}

std::string LogFormatter::Format(LogRecord const& in_record) const noexcept
{
    return ComputeLabel(in_record) + " " + in_record.message + "\n";
}

#pragma endregion