#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct ManualResetEvent
{
	~ManualResetEvent()					      noexcept { m_awaitable.Consume(); }
	RkVoid     Signal(TValue const& in_value) noexcept { m_value = in_value; m_awaitable.Consume(true); }
	RkVoid	   Reset ()						  noexcept { m_awaitable.Reset(); }

	CPUAwaitable<TValue> operator co_await() noexcept { return m_awaitable; }

	private:

		TValue				   m_value			   {};
		CPUContinuationNodePtr m_continuation_node {};
		CPUAwaitable<TValue>   m_awaitable		   {m_continuation_node, &m_value};
};

template <>
struct ManualResetEvent<RkVoid>
{
	~ManualResetEvent() noexcept { m_awaitable.Consume(); }
	RkVoid     Signal() noexcept { m_awaitable.Consume(true); }
	RkVoid	   Reset () noexcept { m_awaitable.Reset(); }

	CPUAwaitable<RkVoid> operator co_await() const noexcept { return m_awaitable; }

	private:

		CPUContinuationNodePtr m_continuation_node {};
		CPUAwaitable<RkVoid>   m_awaitable		   {m_continuation_node};
};

END_RUKEN_NAMESPACE