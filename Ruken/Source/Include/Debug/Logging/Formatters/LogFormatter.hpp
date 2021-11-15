
#pragma once

#include <string>

#include "Debug/Logging/LogRecord.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class configures the final order, structure, and contents of the log message.
 */
class LogFormatter
{
    protected:

        #pragma region Methods

        [[nodiscard]]
        virtual std::string ComputeLabel(LogRecord const& in_record) const noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        LogFormatter() = default;

        LogFormatter(LogFormatter const& in_copy) = delete;
        LogFormatter(LogFormatter&&      in_move) = delete;

        virtual ~LogFormatter() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return The resulting string computed from the record.
         */
        [[nodiscard]]
        virtual std::string Format(LogRecord const& in_record) const noexcept;

        #pragma endregion

        #pragma region Operators

        LogFormatter& operator=(LogFormatter const& in_copy) = delete;
        LogFormatter& operator=(LogFormatter&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE