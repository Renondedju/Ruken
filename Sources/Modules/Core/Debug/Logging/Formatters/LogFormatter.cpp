#include "Core/Debug/Logging/Formatters/LogFormatter.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

std::string LogFormatter::ComputeLabel(LogRecord const& in_record) const noexcept
{
    switch (in_record.level)
    {
        case ELogLevel::Debug:   return "[Debug]  ";
        case ELogLevel::Info:    return "[Info]   ";
        case ELogLevel::Warning: return "[Warning]";
        case ELogLevel::Error:   return "[Error]  ";
        case ELogLevel::Fatal:   return "[Fatal]  ";
    }

    return "";
}

std::string LogFormatter::Format(LogRecord const& in_record) const noexcept
{
    return ComputeLabel(in_record) + " " + in_record.message + "\n";
}

#pragma endregion