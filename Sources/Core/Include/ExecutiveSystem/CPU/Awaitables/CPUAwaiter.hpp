#pragma once

#include <coroutine>

#include "Build/Namespace.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUSignal.hpp"

BEGIN_RUKEN_NAMESPACE

struct CPUAwaiter;

// Awaiter chain node type
using CPUAwaiterList = std::atomic<CPUAwaiter*>;

/**
 * Awaiters are basically a long thread-safe linked list holding
 * each a signal to trigger when the wait is over.
 */
struct CPUAwaiter
{
	static inline auto locked   {reinterpret_cast<CPUAwaiter* const>(0x1)};
	static inline auto consumed {reinterpret_cast<CPUAwaiter* const>(0x2)};
	static inline auto detached {reinterpret_cast<CPUAwaiter* const>(0x3)};

	CPUAwaiterList* head   {nullptr};	///< Reference to the head of the list
	CPUAwaiterList  next   {detached}; ///< Next awaiter in the list
	CPUSignal       signal {};
	RkUint64        tag    {};

	#pragma region Lifetime

	explicit CPUAwaiter(CPUAwaiterList* in_head = nullptr) noexcept;
	CPUAwaiter(CPUAwaiter const&) 						   noexcept;
	CPUAwaiter(CPUAwaiter&&)	  						   noexcept;
	~CPUAwaiter();

	CPUAwaiter& operator=(CPUAwaiter const&) noexcept;
	CPUAwaiter& operator=(CPUAwaiter&&)      noexcept;

	#pragma endregion

	#pragma region Coroutine Methods

	[[nodiscard]] RkBool await_ready  ()                  const noexcept;
	[[nodiscard]] RkBool await_suspend(std::coroutine_handle<>) noexcept;
				  RkVoid await_resume ()                  const noexcept;

	#pragma endregion

	protected:

		/**
		 * @brief  Attempts to attach the awaiter to the currently set head.
		 * @return True if the operation succeeded, false otherwise.
		 */
		RkBool TryAttach() noexcept;
		RkBool TryDetach() noexcept;

		/**
		 * @brief Checks if the awaited event has been consumed already
		 * @note This function can be called even if the awaited event has been deleted already
		 */
		RkBool Consumed() const noexcept;
};

END_RUKEN_NAMESPACE