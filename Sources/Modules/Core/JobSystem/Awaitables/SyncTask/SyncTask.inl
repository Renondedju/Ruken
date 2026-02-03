#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTaskPromise<TResult>& in_parent) noexcept:
	m_handle {std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(in_parent)}
{}

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTask&& in_move) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::swap(m_handle, in_move.m_handle);
}

template<typename TResult>
SyncTask<TResult>& SyncTask<TResult>::operator=(SyncTask&& in_move) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::swap(m_handle, in_move.m_handle);

	return *this;
}

template<typename TResult>
SyncTask<TResult>::~SyncTask()
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_handle && m_handle.promise().references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_handle.destroy();
}

template<typename TResult>
auto SyncTask<TResult>::operator co_await() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return m_handle.promise().operator co_await();
}

END_RUKEN_NAMESPACE