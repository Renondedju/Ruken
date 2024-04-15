
#pragma once

#include "Debug/Logging/Handlers/StreamHandler.hpp"

#include "Debug/Logging/Formatters/ConsoleFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class sends output to the system console.
 */
class ConsoleHandler final : public StreamHandler
{
    /**
     * \brief Kernel is the only class able to create a ConsoleHandler.
     */
    friend class Kernel;

    private:

        #pragma region Members

        ConsoleFormatter m_console_formatter;

        #pragma endregion

        #pragma region Constructors

        ConsoleHandler() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        ConsoleHandler(ConsoleHandler const& in_copy) = delete;
        ConsoleHandler(ConsoleHandler&&      in_move) = delete;

        ~ConsoleHandler() = default;

        #pragma endregion

        #pragma region Operators

        ConsoleHandler& operator=(ConsoleHandler const& in_copy) = delete;
        ConsoleHandler& operator=(ConsoleHandler&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE