
#pragma once

#include "Debug/Logging/LogRecord.hpp"
#include "Build/Attributes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class provides a finer grained facility for determining which log records to output.
 * \note  Filters objects can be used by Loggers for more sophisticated filtering than is provided by levels.
 */
class RUKEN_NO_VTABLE LogFilter
{
    public:

        #pragma region Constructors

        LogFilter() = default;

        LogFilter(LogFilter const& in_copy) = delete;
        LogFilter(LogFilter&&      in_move) = delete;

        virtual ~LogFilter() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if the specified record is to be logged, else False.
         */
        [[nodiscard]]
        virtual RkBool Filter(LogRecord const& in_record) const = 0;

        #pragma endregion

        #pragma region Operators

        LogFilter& operator=(LogFilter const& in_copy) = delete;
        LogFilter& operator=(LogFilter&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE