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
	using SignalValue = TSignalValue;

	static inline auto locked   {reinterpret_cast<CPUAwaiter* const>(0x1)};
	static inline auto consumed {reinterpret_cast<CPUAwaiter* const>(0x2)};
	static inline auto detached {reinterpret_cast<CPUAwaiter* const>(0x3)};

	CPUAwaiterList<TSignalValue>* head;			      ///< Reference to the head of the list
	CPUAwaiterList<TSignalValue>  next   {detached}; ///< Next awaiter in the list
	CPUSignal     <TSignalValue>  signal {};
	RkUint64                      tag    {};

	#pragma region Lifetime

	explicit CPUAwaiter(CPUAwaiterList<TSignalValue>* in_head = nullptr) noexcept;
	CPUAwaiter(CPUAwaiter const&) 										 noexcept;
	CPUAwaiter(CPUAwaiter&&)	  										 noexcept;
	~CPUAwaiter();

	CPUAwaiter& operator=(CPUAwaiter const&) noexcept;
	CPUAwaiter& operator=(CPUAwaiter&&)      noexcept;

	#pragma endregion

	RkBool TryAttach() noexcept;
	RkBool TryDetach() noexcept;
	RkBool Consumed () const noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.inl"