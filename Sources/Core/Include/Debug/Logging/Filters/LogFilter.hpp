#pragma once

#include "Debug/Logging/LogRecord.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class provides a finer grained facility for determining which log records to output.
 * \note  Filters objects can be used by Loggers for more sophisticated filtering than is provided by levels.
 */
struct LogFilter
{
	virtual ~LogFilter() = default;

	/**
	 * \return True if the specified record is to be logged, else False.
	 */
	[[nodiscard]] virtual RkBool Filter(LogRecord const& in_record) const = 0;
};

END_RUKEN_NAMESPACE