#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

struct SharedMutex
{
	enum class EAccessType: RkUint64
	{ Read = 0, Write = 1 };

	#pragma region Members

	EAccessType			   current_access_type {EAccessType::Read};
	std::atomic_uint64_t   concurrency		   {};
	CPUAwaiterList<RkVoid> continuation_node   {};
	CPUAwaitable  <RkVoid> awaitable		   {continuation_node};

	#pragma endregion

	#pragma region Methods

	/**
	 *
	 */
	RkVoid RemoveAccess()
	{
		if (concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		{
			if (current_access_type == EAccessType::Read)
				current_access_type = EAccessType::Write;
			else
				current_access_type = EAccessType::Read;

			awaitable.Consume(true, reinterpret_cast<RkUint64*>(&current_access_type));
		}
	}

	#pragma endregion
};

struct SharedMutexScopedAccess
{
	SharedMutexScopedAccess(SharedMutex& in_mutex, SharedMutex::EAccessType const in_access_type) noexcept:
		mutex      (in_mutex),
		access_type(in_access_type)
	{}

	CPUAwaitable<RkVoid> operator co_await() const
	{
		// TODO try to attach last + insert tag (access_type)
		return mutex.awaitable;
	}

	~SharedMutexScopedAccess()
	{
		// Decrement the concurrency

	}

	SharedMutex&			 mutex;
	SharedMutex::EAccessType access_type;
};

END_RUKEN_NAMESPACE
