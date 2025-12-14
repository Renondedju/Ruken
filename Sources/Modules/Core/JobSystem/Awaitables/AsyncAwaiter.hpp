#pragma once

#include "Build/Namespace.hpp"
#include "JobSystem/Awaitables/SignalReceiver.hpp"

#include <atomic>
#include <coroutine>

BEGIN_RUKEN_NAMESPACE

// Awaiter chain node type
using AsyncAwaiterList = std::atomic<struct AsyncAwaiter*>;

/**
 * @brief An object that waits for the completion of an awaitable.
 * Awaiters are a long thread-safe linked list owned by the awaited Awaitable, each node holds
 * a callback (.signal) to trigger when the Awaitable is done or triggered.
 *
 * Awaitables are responsible for implementing the actual signal/consuming
 * algorythm and may or may not use the .tag value to do so.
 *
 * If an awaitable can be destroyed before all of it's attached awaiters,
 * you must make sure its .next pointer is set to Awaiter::consumed.
 */
struct AsyncAwaiter
{
	static inline auto const locked   {reinterpret_cast<AsyncAwaiter*>(0x1)};
	static inline auto const consumed {reinterpret_cast<AsyncAwaiter*>(0x2)};
	static inline auto const detached {reinterpret_cast<AsyncAwaiter*>(0x3)};

	AsyncAwaiterList*   head   {nullptr};   ///< Reference to the head of the list
	AsyncAwaiterList    next   {detached}; ///< Next awaiter in the list
	SignalReceiver signal {};		 ///< Callback to trigger when the wait is over
	RkUint64       tag    {};       ///< User defined value used in the implementation of the signal/consuming algorythm

	#pragma region Lifetime

	explicit AsyncAwaiter  (AsyncAwaiterList* in_head = nullptr) noexcept;
	AsyncAwaiter		   (AsyncAwaiter const&) noexcept;
	AsyncAwaiter		   (AsyncAwaiter&&)	     noexcept;
	AsyncAwaiter& operator=(AsyncAwaiter const&) noexcept;
	AsyncAwaiter& operator=(AsyncAwaiter&&)      noexcept;
	~AsyncAwaiter();

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