#pragma once
#include <iostream>

BEGIN_RUKEN_NAMESPACE
	template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTaskPromise<TResult>& in_parent) noexcept:
	m_promise {&in_parent}
{}

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTask&& in_move) noexcept:
	m_promise {std::exchange(in_move.m_promise, nullptr)}
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));
}

template<typename TResult>
SyncTask<TResult>& SyncTask<TResult>::operator=(SyncTask&& in_move) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_promise && m_promise->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		std::coroutine_handle<promise_type>::from_promise(*m_promise).destroy();

	m_promise = std::exchange(in_move.m_promise, nullptr);

	return *this;
}

template<typename TResult>
SyncTask<TResult>::~SyncTask() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_promise && m_promise->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		std::coroutine_handle<promise_type>::from_promise(*m_promise).destroy();
}

template<typename TResult>
auto SyncTask<TResult>::operator co_await() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return m_promise->operator co_await();
}

END_RUKEN_NAMESPACE