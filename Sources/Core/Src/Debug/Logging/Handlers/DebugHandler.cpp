#include "Debug/Logging/Handlers/DebugHandler.hpp"
#include "Build/BuildInfo.hpp"

#include <tracy/Tracy.hpp>

USING_RUKEN_NAMESPACE

DebugHandler::DebugHandler(LogFormatter const& in_formatter) noexcept:
	LogHandler(in_formatter)
{}

RkVoid DebugHandler::Handle([[maybe_unused]] LogRecord&& in_record) noexcept
{
	RkUint32 color {};
	switch (in_record.level)
	{
		case ELogLevel::Debug: // Green-yellow
			color = 0xADFF2F; break;
		case ELogLevel::Info: // White
			color = 0xFFFFFF; break;
		case ELogLevel::Warning: // Gold
			color = 0xFFD700; break;
		case ELogLevel::Fatal:
		case ELogLevel::Error: // Red
			color = 0xFF0000; break;
	}

	TracyMessageC(in_record.message.c_str(), in_record.message.size(), color);
/*
	if (BuildInfo:: &&
		(in_record.level == ELogLevel::Fatal && BuildInfo::BreakOnError ||
		(in_record.level == ELogLevel::Error && BuildInfo::BreakOnError)))
		__debugbreak();
	*/
}
