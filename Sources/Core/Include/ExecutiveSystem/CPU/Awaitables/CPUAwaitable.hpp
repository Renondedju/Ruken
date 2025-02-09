#pragma once

#include "ExecutiveSystem/Awaitable.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"

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
	static constexpr RkBool has_value { !std::is_void_v<TValue> };

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_list_head Continuation hook reference.
	 * @param in_value Value reference.
	 */
	explicit CPUAwaitable(CPUAwaiterList<TValue>& in_list_head, TValue* in_value) noexcept requires ( has_value);
	explicit CPUAwaitable(CPUAwaiterList<TValue>& in_list_head			        ) noexcept requires (!has_value);

	CPUAwaitable ()						     = default;
	CPUAwaitable (CPUAwaitable const& other) = default;
	CPUAwaitable (CPUAwaitable&&      other) = default;
	~CPUAwaitable()							 = default;

	CPUAwaitable& operator=(CPUAwaitable const& other) = default;
	CPUAwaitable& operator=(CPUAwaitable&&      other) = default;

	#pragma endregion

	#pragma region Methods

	/// @return Returns true if the event has already been completed, false otherwise
	[[nodiscard]] RkBool Completed() const noexcept;

	template <typename TOtherValue> RkBool TryAttach(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept;
	template <typename TOtherValue> RkBool TryDetach(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept;

	/**
	 * @brief Calls TryAttach and Signals the awaiter automatically if the operation failed
	 *
	 * @todo { We could optimize attach bulk by passing a pre-constructed chain of awaiters }
	 */
	template <typename TOtherValue>
	requires (std::is_void_v<TOtherValue> || std::is_same_v<TOtherValue, TValue>)
	RkVoid AttachOrSignal(CPUAwaiter<TOtherValue>& in_awaiter) const noexcept;

	/**
	 * @brief Signals the completion to all the attached awaiters.
	 * Reset() does not need to be called and attached continuations are not consumed.
	 *
	 * @param in_value pointer to the value to be signaled
	 * @param in_tag When set, the function will only signal awaiters that have the same tag value
	 */
	RkVoid Signal(TValue* in_value, RkUint64* in_tag = nullptr) noexcept requires ( has_value);
	RkVoid Signal(					RkUint64* in_tag = nullptr) noexcept requires (!has_value);

	/**
	 * @brief Signals the completion of the event and consumes all the attached continuations.
	 * The awaitable will become completed until Reset() has been called.
	 *
	 * @param in_signal When set to true, the awaiter will be signaled at the same time and in_value must be a valid pointer.
	 * @param in_value Pointer to the value to be signaled, if in_signal is set to true.
	 * @param in_tag When set, the function will only consume until it finds an awaiter with a different tag value.
	 */
	RkVoid Consume(RkBool in_signal = false, TValue* in_value = nullptr, RkUint64* in_tag = nullptr) noexcept requires ( has_value);
	RkVoid Consume(RkBool in_signal = false							   , RkUint64* in_tag = nullptr) noexcept requires (!has_value);

	/**
	 * @brief Resets the event, making it non consumed and ready to hook new awaiters.
	 *
	 * Do note that any awaiters will be lost and won't be resumed if reset is called when
	 * CPUAwaitable::Completed() returns false. Make sure to signal a completion before resetting.
	 */
	RkVoid Reset() noexcept;

	#pragma endregion

	private:

		CPUAwaiterList<TValue>* m_awaiter_list {nullptr};

		// Actual implementations of signal and consume
		RkVoid DoSignal (				   RkUint64* in_tag = nullptr) noexcept;
		RkVoid DoConsume(RkBool in_signal, RkUint64* in_tag = nullptr) noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.inl"