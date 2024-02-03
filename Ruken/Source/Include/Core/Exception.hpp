#pragma once

#include <exception>
#include <stacktrace>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief The base exception
 */
struct Exception : std::exception
{
	std::stacktrace stacktrace;

	/**
	 * \brief Default constructor
	 * \param in_what Reason of the exception
	 * \param in_stacktrace The stacktrace of the code that created this exception
	 */
	explicit Exception(const std::string_view in_what, std::stacktrace const& in_stacktrace = std::stacktrace::current()):
		std::exception (in_what.data()),
		stacktrace     (in_stacktrace)
	{}
};

END_RUKEN_NAMESPACE