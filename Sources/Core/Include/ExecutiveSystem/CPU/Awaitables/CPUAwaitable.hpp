#pragma once

#include "ExecutiveSystem/Awaitable.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Continuations/CPUContinuationNode.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

#pragma region Utilities

template <typename TValue> struct CPUAwaitableStorage		  { TValue value {}; };
template <               > struct CPUAwaitableStorage<RkVoid> {};

#pragma endregion

/**
 * @brief This class acts as a base for every CPU awaitable and is meant to be inherited from.
 *
 * Continuations will attempt to detach themselves upon destruction if the event has not been consumed.
 * If an awaitable can be destroyed before all of it's attached continuations,
 * you must make sure it is consumed by calling CPUAwaitable::Consume().
 */
template <typename TValue>
struct CPUAwaitable : protected CPUAwaitableStorage<TValue*>, Awaitable<CentralProcessingUnit>
{
	using ValueT = TValue;

	/// @return Returns true if the event has already been completed, false otherwise
	[[nodiscard]] RkBool                  Completed          () const noexcept;

	/// @return Returns the continuation node used to await this awaitable
	[[nodiscard]] CPUContinuationNodePtr* GetContinuationHook() const noexcept;


	static constexpr RkBool has_value { !std::is_same_v<TValue, RkVoid> };

	protected:

		#pragma region Lifetime

		/**
		 * Default constructor.
		 * @param in_continuation_hook Continuation hook reference.
		 * @param in_value Value reference.
		 */
		explicit CPUAwaitable(CPUContinuationNodePtr& in_continuation_hook, TValue* in_value) noexcept requires ( has_value);
		explicit CPUAwaitable(CPUContinuationNodePtr& in_continuation_hook			        ) noexcept requires (!has_value);

		CPUAwaitable ()						     = default;
		CPUAwaitable (CPUAwaitable const& other) = default;
		CPUAwaitable (CPUAwaitable&&      other) = default;
		~CPUAwaitable()							 = default;

		CPUAwaitable& operator=(CPUAwaitable const& other) = default;
		CPUAwaitable& operator=(CPUAwaitable&&      other) = default;

		#pragma endregion

		/**
		 * @brief Signals the completion to all the attached awaiters.
		 * Reset() does not need to be called and attached continuations are not consumed.
		 *
		 * @param in_value pointer to the value to be signaled
		 */
		RkVoid Signal(TValue* in_value) noexcept requires ( has_value);
		RkVoid Signal()				    noexcept requires (!has_value);

		/**
		 * @brief Signals the completion of the event and consumes all the attached continuations.
		 * The awaitable will become completed until Reset() has been called.
		 *
		 * note: if in_signal is true then in_value must be set to a valid pointer.
		 */
		RkVoid Consume(RkBool in_signal = false, TValue* in_value = nullptr) noexcept requires ( has_value);
		RkVoid Consume(RkBool in_signal = false)				             noexcept requires (!has_value);

		/**
		 * @brief Resets the event, making it non consumed and ready to hook new awaiters.
		 *
		 * Do note that any awaiters will be lost and won't be resumed if reset is called when
		 * CPUAwaitable::Completed() returns false. Make sure to signal a completion before resetting.
		 */
		RkVoid Reset() noexcept;

	private:

		CPUContinuationNodePtr* m_continuation_hook {};

		// Actual implementations of signal and consume
		RkVoid DoSignal ()				   noexcept;
		RkVoid DoConsume(RkBool in_signal) noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl"