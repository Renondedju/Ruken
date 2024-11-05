#pragma once

#include "Build/Namespace.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUSignal.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct CPUAwaiter;

template <typename TValue>
using CPUAwaiterList = std::atomic<CPUAwaiter<TValue>*>;

/**
 * Awaiters are basically a long thread-safe linked list holding
 * each a signal to trigger when the wait is over.
 *
 * @tparam TSignalValue The expected value type to be signaled by the awaitable.
 */
template <typename TSignalValue>
struct CPUAwaiter
{
	// FIXME: /!\ /!\ /!\ /!\ /!\ /!\
	// FIXME: If deleted, awaiters won't detach themselves -> Read after free errors

	static inline auto locked    {reinterpret_cast<CPUAwaiter* const>(0x1)};
	static inline auto consumed  {reinterpret_cast<CPUAwaiter* const>(0x2)};

	RkBool Consumed() const noexcept
	{ return head && head->load(std::memory_order_acquire) == consumed; }

	CPUAwaiterList<TSignalValue>* head   {nullptr};  ///< Reference to the head of the list
	CPUAwaiterList<TSignalValue>  next   {nullptr}; ///< Next awaiter in the list

	CPUSignal     <TSignalValue>  signal {};
	RkUint64                      tag    {};
};

END_RUKEN_NAMESPACE