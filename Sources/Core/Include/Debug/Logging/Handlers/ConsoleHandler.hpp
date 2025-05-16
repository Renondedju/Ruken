#pragma once

#include "Debug/Logging/Handlers/StreamHandler.hpp"

#include "Debug/Logging/Formatters/ConsoleFormatter.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class sends output to the system console.
 */
class ConsoleHandler final : public StreamHandler
{
	ConsoleFormatter m_console_formatter;

    public:

        #pragma region Lifetime

        ConsoleHandler() noexcept;
        ConsoleHandler(ConsoleHandler const& in_copy) = delete;
        ConsoleHandler(ConsoleHandler&&      in_move) = delete;

		ConsoleHandler& operator=(ConsoleHandler const& in_copy) = delete;
		ConsoleHandler& operator=(ConsoleHandler&&      in_move) = delete;

        ~ConsoleHandler() override = default;

        #pragma endregion

		RkVoid Handle(LogRecord&& in_record) noexcept override;
};

END_RUKEN_NAMESPACE