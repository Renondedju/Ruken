
#pragma once

#include "Debug/Logging/Formatters/LogFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class configures the final order, structure, and contents of the log message for the console.
 */
class ConsoleFormatter final : public LogFormatter
{
    /**
     * \brief ConsoleHandler is the only class able to create a ConsoleFormatter.
     */
    friend class ConsoleHandler;

    private:

        #pragma region Members

        RkVoid* m_handle {nullptr};

        #pragma endregion

        #pragma region Constructors

        ConsoleFormatter() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        std::string ComputeLabel(LogRecord const& in_record) const noexcept override;

        #pragma endregion

    public:

        #pragma region Constructors

        ConsoleFormatter(ConsoleFormatter const& in_copy) = delete;
        ConsoleFormatter(ConsoleFormatter&&      in_move) = delete;

        ~ConsoleFormatter() = default;

        #pragma endregion

        #pragma region Operators

        ConsoleFormatter& operator=(ConsoleFormatter const& in_copy) = delete;
        ConsoleFormatter& operator=(ConsoleFormatter&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE