#pragma once

#include "Core/Debug/Trace.hpp"
#include "Core/Debug/Exception.hpp"

#include <coroutine>

BEGIN_RUKEN_NAMESPACE

/// @brief A utility class inherited by task types to ease the implementation of automatic tracing
struct CoroutineTracingUtils
{
	#ifdef RUKEN_TRACE_BUILD
		TracyCZoneCtx zone {};
	#endif

	static RkVoid ReportException(std::exception_ptr const& in_ptr) noexcept
	{
		// TODO: Add a way to trace exception propagation without tracy
		//		 I am thinking of a list of source locations to construct an async callstack

		try {
			std::rethrow_exception(in_ptr);
		}
		catch (Exception& in_exception) {
			std::string const what {std::format("{}", in_exception)};
			TracyMessageC(what.c_str(), what.length(), 0xFF0000);
		}
	}

	// Each of these awaiters overloads TInherited to automatically maintain the tracy zone.
	#pragma region Awaiters

	template <typename TInherited = std::suspend_always>
	struct InitialSuspendAwaiter: TInherited
	{
		CoroutineTracingUtils* promise;
		std::source_location   start_location;

		auto await_resume() const noexcept(noexcept(TInherited::await_resume()))
		{ TRACY_BEGIN_ZONE(promise->zone, start_location, true); return TInherited::await_resume(); }
	};

	template <typename TInherited = std::suspend_always>
	struct AwaitTransformAwaiter: TInherited
	{
		CoroutineTracingUtils* promise;
		std::source_location   await_location;

		auto await_ready()  const noexcept(noexcept(TInherited::await_ready()))
		{ TRACY_END_ZONE(promise->zone); return TInherited::await_ready(); }

		auto await_resume() const noexcept(noexcept(TInherited::await_resume()))
		{
			TRACY_BEGIN_ZONE(promise->zone, await_location, true);

			if constexpr (std::is_void_v<decltype(TInherited::await_resume())>)
				TInherited::await_resume();
			else
				return TInherited::await_resume();
		}
	};

	template <typename TInherited = std::suspend_always>
	struct FinalSuspendAwaiter: TInherited
	{
		CoroutineTracingUtils* promise;

		auto await_ready() const noexcept(noexcept(TInherited::await_ready()))
		{ TRACY_END_ZONE(promise->zone); return TInherited::await_ready(); }
	};

	#pragma endregion
};

END_RUKEN_NAMESPACE