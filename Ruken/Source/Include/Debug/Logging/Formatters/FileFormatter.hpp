
#pragma once

#include "Debug/Logging/Formatters/LogFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class configures the final order, structure, and contents of the log message for files.
 */
class FileFormatter final : public LogFormatter
{
    public:

        #pragma region Constructors

        FileFormatter() = default;

        FileFormatter(FileFormatter const& in_copy) = delete;
        FileFormatter(FileFormatter&&      in_move) = delete;

        ~FileFormatter() = default;

        #pragma endregion

        #pragma region Operators

        FileFormatter& operator=(FileFormatter const& in_copy) = delete;
        FileFormatter& operator=(FileFormatter&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE