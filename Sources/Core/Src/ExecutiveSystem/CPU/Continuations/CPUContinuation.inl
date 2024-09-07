#pragma once

#include "ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
RkVoid CPUContinuation<TValue>::Signal(TValue const* in_value) const noexcept requires (has_value)
{
	m_signal_operation(m_awaiter_ptr, reinterpret_cast<void const*>(in_value));
}

template <typename TValue>
RkVoid CPUContinuation<TValue>::Signal() const noexcept requires (!has_value)
{
	m_signal_operation(m_awaiter_ptr, nullptr);
}

END_RUKEN_NAMESPACE