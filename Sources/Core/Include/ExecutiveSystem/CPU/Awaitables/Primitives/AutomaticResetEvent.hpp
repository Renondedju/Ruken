#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct AutomaticResetEvent: CPUAwaitable<TValue>
{
	using Parent = CPUAwaitable<TValue>;

	 AutomaticResetEvent()						 noexcept: Parent(m_continuation_node, &m_value) {}
	~AutomaticResetEvent()					     noexcept {					    Parent::Consume(); }
	RkVoid        Signal(TValue const& in_value) noexcept { m_value = in_value; Parent::Signal(); }
	RkVoid	      Reset ()						 noexcept {					    Parent::Reset(); }

private:

	TValue				   m_value			   {};
	CPUContinuationNodePtr m_continuation_node {};
};

template <>
struct AutomaticResetEvent<RkVoid>: CPUAwaitable<RkVoid>
{
	 AutomaticResetEvent() noexcept: CPUAwaitable(m_continuation_node) {}
	~AutomaticResetEvent() noexcept { Consume(); }
	RkVoid        Signal() noexcept { CPUAwaitable::Signal(); }
	RkVoid	      Reset () noexcept { CPUAwaitable::Reset(); }

private:

	CPUContinuationNodePtr m_continuation_node {};
};

END_RUKEN_NAMESPACE