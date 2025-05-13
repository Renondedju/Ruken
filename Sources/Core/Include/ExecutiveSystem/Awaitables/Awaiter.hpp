#pragma once

#include "Build/Namespace.hpp"
#include "ExecutiveSystem/Awaitables/SignalReceiver.hpp"

#include <atomic>
#include <coroutine>

BEGIN_RUKEN_NAMESPACE

struct Awaiter;

// Awaiter chain node type
using AwaiterList = std::atomic<Awaiter*>;

/**
 *
 * @brief An object that waits for the completion of an awaitable.
 * Awaiters are responsible for implementing the actual waiting algorithm.
 * This synchronization between tasks and events should not be mixed up with the content of events themselves.
 *
 * Awaiters are basically a long thread-safe linked list holding
 * each a signal to trigger when the wait is over.
 */
struct Awaiter
{
	static inline auto const locked   {reinterpret_cast<Awaiter* const>(0x1)};
	static inline auto const consumed {reinterpret_cast<Awaiter* const>(0x2)};
	static inline auto const detached {reinterpret_cast<Awaiter* const>(0x3)};

	AwaiterList*   head   {nullptr};	///< Reference to the head of the list
	AwaiterList    next   {detached}; ///< Next awaiter in the list
	SignalReceiver signal {};
	RkUint64       tag    {};

	#pragma region Lifetime

	explicit Awaiter(AwaiterList* in_head = nullptr) noexcept;
	Awaiter(Awaiter const&) 						 noexcept;
	Awaiter(Awaiter&&)	  						     noexcept;
	~Awaiter();

	Awaiter& operator=(Awaiter const&) noexcept;
	Awaiter& operator=(Awaiter&&)      noexcept;

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