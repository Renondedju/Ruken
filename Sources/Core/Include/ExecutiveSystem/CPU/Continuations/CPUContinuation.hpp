#pragma once

#include "ExecutiveSystem/CPU/Continuations/CPUContinuationNode.hpp"
#include "Meta/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct CPUContinuation
{
	static constexpr RkBool has_value {!std::is_same_v<TValue, RkVoid>};

	using ValueT           = TValue;
	using TValueRef		   = std::add_lvalue_reference_t<std::add_const_t<TValue>>;
	using TSignalOperation = RkVoid(RkVoid*, RkVoid const*);

	#pragma region Lifetime

	CPUContinuation()						= default;
	CPUContinuation(const CPUContinuation&) = default;
	CPUContinuation(CPUContinuation&&)		= default;

	CPUContinuation& operator=(const CPUContinuation&) = default;
	CPUContinuation& operator=(CPUContinuation&&     ) = default;

	~CPUContinuation()
	{ m_continuation_node.TryStopAwait(); }

	#pragma endregion

	template <typename TAwaitable, typename TAwaiter>
	RkVoid Setup(TAwaitable const& in_awaitable, TAwaiter& in_awaiter) noexcept
	{
		m_continuation_node.head = in_awaitable.GetContinuationHook();
		m_awaiter_ptr		     = std::addressof(in_awaiter);
		m_signal_operation       = [](RkVoid* in_awaiter_ptr, RkVoid const* in_value) {
			if constexpr ( has_value && std::is_same_v<typename TAwaiter::ValueT, TValue>)
				static_cast<TAwaiter*>(in_awaiter_ptr)->Signal(*static_cast<TValue const*>(in_value));
			if constexpr (!has_value || std::is_same_v<typename TAwaiter::ValueT, RkVoid>)
				static_cast<TAwaiter*>(in_awaiter_ptr)->Signal();
		};
	}

	RkBool TryStartAwait() noexcept
	{
		RUKEN_ASSERT(m_awaiter_ptr != nullptr, "Awaiter ptr should not be null, make sure Setup() is properly called.");

		return m_continuation_node.TryStartAwait();
	}

	/**
	 * Signals the owner (awaiter) of the continuation.
	 * @note The underlying logic is type erased, meaning that the owning awaiter can discard any passed value if needed.
	 */
	RkVoid Signal(TValue const* in_value) const noexcept requires ( has_value);
	RkVoid Signal()				          const noexcept requires (!has_value);

	RkBool Consumed() const noexcept
	{ return m_continuation_node.Consumed(); }

	private:

		CPUContinuationNode m_continuation_node {};
		RkVoid*    			m_awaiter_ptr		{};
		TSignalOperation* 	m_signal_operation  {};
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Continuations/CPUContinuation.inl"