#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

#include <memory>
#include <type_traits>

BEGIN_RUKEN_NAMESPACE

// Checks if a type defines TType::Signal() -> RkVoid
template <typename TType>
concept CSignal	= requires (TType& in_type) { { in_type.Signal() } -> std::convertible_to<RkVoid>; };

/**
 * @brief A utility used to store any type that has a Signal() member function.
 */
struct SignalReceiver
{
	template <typename TSignal>
	explicit SignalReceiver(TSignal& in_signal_instance) noexcept requires CSignal<TSignal>:
		m_instance_ptr	   {std::addressof(in_signal_instance)},
		m_signal_operation {[](RkVoid* in_awaiter_ptr) {
				static_cast<TSignal*>(in_awaiter_ptr)->Signal();
		}}
	{}

	SignalReceiver()			  			 = default;
	SignalReceiver(const SignalReceiver&) 			 = default;
	SignalReceiver(SignalReceiver&&     ) 			 = default;
	SignalReceiver& operator=(const SignalReceiver&) = default;
	SignalReceiver& operator=(SignalReceiver&&     ) = default;

	RkVoid Invoke() const
	{ m_signal_operation(m_instance_ptr); }

	private:

		RkVoid* m_instance_ptr				 {};
		RkVoid(*m_signal_operation)(RkVoid*) {};
};

END_RUKEN_NAMESPACE