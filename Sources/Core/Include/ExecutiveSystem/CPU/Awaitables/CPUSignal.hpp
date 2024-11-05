#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

#include <memory>
#include <type_traits>

BEGIN_RUKEN_NAMESPACE

// Checks if a type defines TType::Signal() -> RkVoid
template <typename TType>
concept CSignal		 = requires (TType& in_type) { { in_type.Signal() } -> std::convertible_to<RkVoid>; };

// Checks if a type defines TType::Signal(TValue&) -> RkVoid
template <typename TType, typename TValue>
concept CSignalValue = requires (TType& in_type, TValue& in_value) { { in_type.Signal(in_value) } -> std::convertible_to<RkVoid>; };

/**
 * Awaiter handles are type erased awaiters. You can think of it as a function pointer.
 * @tparam TValue The type that will be signaled by an awaitable.
 *				  There are 2 scenarios:
 *				    - TValue is void then only CPUAwaiter<void>* is accepted as a valid pointer.
 *			        - TValue is any other type then CPUAwaiter<void>* and CPUAwaiter<TValue>* are considered valid pointers.
 */
template <typename TValue>
struct CPUSignal
{
	CPUSignal()				    = default;
	CPUSignal(const CPUSignal&) = default;
	CPUSignal(CPUSignal&&)	    = default;

	CPUSignal& operator=(const CPUSignal&) = default;
	CPUSignal& operator=(CPUSignal&&     ) = default;

	template <typename TSignal>
	explicit CPUSignal(TSignal& in_signal_instance) noexcept
		requires CSignal<TSignal> || CSignalValue<TSignal, TValue>:

		m_instance_ptr	   {std::addressof(in_signal_instance)},
		m_signal_operation {[](RkVoid* in_awaiter_ptr, RkVoid const* in_value) {
			if constexpr (!std::is_void_v<TValue> && CSignalValue<TSignal, TValue>)
				static_cast<TSignal*>(in_awaiter_ptr)->Signal(*static_cast<TValue const*>(in_value));
			if constexpr ( CSignal<TSignal>)
				static_cast<TSignal*>(in_awaiter_ptr)->Signal();
		}}
	{}

	RkVoid Signal(TValue const* in_value) const noexcept requires (!std::is_void_v<TValue>)
	{ m_signal_operation(m_instance_ptr, reinterpret_cast<RkVoid const*>(in_value)); }

	RkVoid Signal()				          const noexcept requires ( std::is_void_v<TValue>)
	{ m_signal_operation(m_instance_ptr, nullptr); }

	private:

		RkVoid* m_instance_ptr								{};
		RkVoid(*m_signal_operation)(RkVoid*, RkVoid const*) {};
};

END_RUKEN_NAMESPACE