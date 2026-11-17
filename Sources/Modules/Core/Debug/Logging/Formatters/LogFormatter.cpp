#include "Core/Debug/Logging/Formatters/LogFormatter.hpp"

#include <ranges>
#include <format>

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
    }

    return "";
}

std::string LogFormatter::Format(LogRecord const& in_record) const noexcept
{
    std::string prefix = ComputeLabel(in_record);

    auto prefixed_view = in_record.message
        | std::views::split('\n')
        | std::views::join_with(std::format("\n{} ", prefix));

    return std::format("{}{}\n", prefix, std::string(std::from_range, prefixed_view));
}

#pragma endregion