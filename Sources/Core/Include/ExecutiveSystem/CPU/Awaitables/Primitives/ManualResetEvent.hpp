#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct ManualResetEvent: CPUAwaitable<TValue>
{
	using Parent = CPUAwaitable<TValue>;

	 ManualResetEvent()						  noexcept: Parent(m_continuation_node, &m_value) {}
	~ManualResetEvent()					      noexcept {					 Parent::Consume(); }
	RkVoid     Signal(TValue const& in_value) noexcept { m_value = in_value; Parent::Consume(true); }
	RkVoid	   Reset ()						  noexcept {					 Parent::Reset(); }

	private:

		TValue				   m_value			   {};
		CPUContinuationNodePtr m_continuation_node {};
};

template <>
struct ManualResetEvent<RkVoid>: CPUAwaitable<RkVoid>
{
	 ManualResetEvent() noexcept: CPUAwaitable(m_continuation_node) {}
	~ManualResetEvent() noexcept { Consume(); }
	RkVoid     Signal() noexcept { Consume(true); }
	RkVoid	   Reset () noexcept { CPUAwaitable::Reset(); }

	private:

		CPUContinuationNodePtr m_continuation_node {};
};

END_RUKEN_NAMESPACE