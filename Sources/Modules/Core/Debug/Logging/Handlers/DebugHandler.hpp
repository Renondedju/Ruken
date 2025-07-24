#pragma once

#include "Core/Debug/Logging/Handlers/LogHandler.hpp"

BEGIN_RUKEN_NAMESPACE

struct DebugHandler final: LogHandler
{
	#pragma region Lifetime

	explicit DebugHandler(LogFormatter const& in_formatter) noexcept;
	 DebugHandler(const DebugHandler&) 			  = delete;
	 DebugHandler(DebugHandler&&     ) 			  = delete;
	 DebugHandler& operator=(const DebugHandler&) = delete;
	 DebugHandler& operator=(DebugHandler&&     ) = delete;
	~DebugHandler() override = default;

	#pragma endregion

	RkVoid Handle(LogRecord&& in_record) noexcept override;
};

END_RUKEN_NAMESPACE