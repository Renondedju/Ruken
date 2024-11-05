#pragma once

#include "Build/Namespace.hpp"

#include <string>
#include <format>
#include <stacktrace>
#include <source_location>
#include <magic_enum.hpp>

BEGIN_RUKEN_NAMESPACE

/// The base exception
struct Exception
{
	std::string                reason;
	const std::stacktrace	   stacktrace;
	const std::source_location source_location;

	/**
	 * @brief Default constructor
	 * @param in_what Reason of the exception
	 * @param in_stacktrace The stacktrace of the code that created this exception
	 * @param in_source_location The source location of the code that created this exception
	 */
	explicit Exception(const std::string_view in_what,
						std::stacktrace		 const& in_stacktrace	   = std::stacktrace::current(),
						std::source_location const& in_source_location = std::source_location::current()):
		reason			{in_what},
		stacktrace      {in_stacktrace},
		source_location {in_source_location}
	{}

	/// Pretty printing
	explicit operator std::string() const noexcept
	{
		return std::format("Exception caught: {}\n{}", reason, stacktrace);
	}
};

END_RUKEN_NAMESPACE