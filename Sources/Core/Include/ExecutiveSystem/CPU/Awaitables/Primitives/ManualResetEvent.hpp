#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct ManualResetEvent: CPUAwaitable<TValue>
{
	ManualResetEvent ()						  noexcept: CPUAwaitable<TValue> {m_continuation_node, &m_value} {}
	~ManualResetEvent()					      noexcept { CPUAwaitable<TValue>::Consume(); }
	RkVoid     Signal(TValue const& in_value) noexcept { m_value = in_value; CPUAwaitable<TValue>::Consume(true); }
	RkVoid	   Reset ()						  noexcept { CPUAwaitable<TValue>::Reset(); }

	private:

		TValue				   m_value			   {};
		CPUAwaiterList<TValue> m_continuation_node {};
};

template <>
struct ManualResetEvent<RkVoid>: CPUAwaitable<RkVoid>
{
	ManualResetEvent () noexcept: CPUAwaitable {m_continuation_node} {}
	~ManualResetEvent() noexcept { Consume(); }
	RkVoid     Signal() noexcept { Consume(true); }
	RkVoid	   Reset () noexcept { CPUAwaitable::Reset(); }

	private:

		CPUAwaiterList<RkVoid> m_continuation_node {};
};

END_RUKEN_NAMESPACE