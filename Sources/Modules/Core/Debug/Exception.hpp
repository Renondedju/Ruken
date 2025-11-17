#pragma once

#include "Core/Build/Namespace.hpp"

#include <string>
#include <format>
#include <stacktrace>
#include <source_location>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief The base exception
struct Exception
{
	std::string                reason;
	const std::stacktrace	   stacktrace;
	const std::source_location source_location;

	/**
	 * @brief Default constructor.
	 * @param in_what Reason of the exception.
	 * @param in_stacktrace The stacktrace of the code that created this exception.
	 * @param in_source_location The source location of the code that created this exception.
	 */
	explicit Exception(std::string&&			   in_what,
	                   std::stacktrace		const& in_stacktrace	  = std::stacktrace::current(),
	                   std::source_location const& in_source_location = std::source_location::current());
};

/// @brief Exception for errno errors.
struct ErrnoException: Exception
{
	/**
	 * @brief Default constructor
	 * @param in_error_number Error number.
	 * @param in_stacktrace The stacktrace of the code that created this exception
	 * @param in_source_location The source location of the code that created this exception
	 */
	explicit ErrnoException(RkErrno				        in_error_number,
	                        std::stacktrace		 const& in_stacktrace	   = std::stacktrace::current(),
	                        std::source_location const& in_source_location = std::source_location::current());

	/**
	 * Converts an error number into a string.
	 * @param in_error_number Error number.
	 * @return Error string.
	 */
	static std::string GetErrorString(RkErrno in_error_number) noexcept;
};

END_RUKEN_NAMESPACE

template <>
struct std::formatter<RUKEN_NAMESPACE::Exception>: std::formatter<std::string>
{
	auto format(RUKEN_NAMESPACE::Exception in_exception, format_context& in_ctx) const
	{
		return std::formatter<string>::format(
			std::format("Exception thrown: {}\n{}", in_exception.reason, in_exception.stacktrace)
			, in_ctx);
	}
};

/*
 * I had some fun trying to implement a python like format for exceptions.
 * It is super slow and would only really be appropriate for a console logger

template <>
struct std::formatter<RUKEN_NAMESPACE::Exception>: std::formatter<std::string>
{
	static inline auto to_string = [](auto p){
		auto c = p | views::common;
		return string(c.begin(), c.end());
	};

	std::string read_file(std::string_view const& in_path, RkUint32 in_line) const
	{
		std::ifstream file(std::filesystem::path(in_path), std::ios::binary);

		if (!file.is_open())
			return "";

		RkUint32 const start_line {std::max<RkUint32>(0, in_line - 2)};

		std::string line;
		for (int i = 1; i < start_line; i++)
			std::getline(file, line);

		std::string str;
		for (int i = start_line; i < start_line + 5 && std::getline(file, line); i++)
			str += std::format("│ {} {} {}\n", i == in_line ? "❱" : " ", i, line);

		return str + "│\n";
	}

	auto format(RUKEN_NAMESPACE::Exception in_exception, format_context& in_ctx) const
	{
		auto str = std::format("{}\n╭{:─^98}╮\n", in_exception.reason, "Traceback (most recent call first)");
		for (auto [index, entry]: views::enumerate(in_exception.stacktrace))
		{
			using std::operator""sv;
			auto header = std::format("{}> {}:{}\nin {}\n", index, entry.source_file(), entry.source_line(), entry.description());
			for (auto subrange: header | views::split("\n"sv) | views::transform(to_string) | views::chunk(96))
				for (auto const& string: subrange)
					str += std::format("│ {: <96} │\n", string);

			str += read_file(entry.source_file(), entry.source_line());
		}
		str += std::format("╰{:─^98}╯", "");

		return std::formatter<string>::format(str, in_ctx);
	}
};
*/