#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTaskPromise<TResult>& in_parent) noexcept:
	m_handle {std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(in_parent)},
	m_parent {std::addressof(in_parent)}
{}

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTask&& in_move) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::swap(m_handle, in_move.m_handle);
	std::swap(m_parent, in_move.m_parent);
}

template<typename TResult>
SyncTask<TResult>& SyncTask<TResult>::operator=(SyncTask&& in_move) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	std::swap(m_handle, in_move.m_handle);
	std::swap(m_parent, in_move.m_parent);

	return *this;
}

template<typename TResult>
SyncTask<TResult>::~SyncTask()
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// If coroutine is not done before destruction, that means that is has not been co_awaited.
	// Therefore, it must be run to completion before destruction of the handle for safety.
	// This also allows to start a synchronous task inside regular functions.
	if (m_parent && !m_handle.done())
		m_handle.resume();
}

template<typename TResult>
SyncTaskAwaiter<TResult> SyncTask<TResult>::operator co_await() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	// Address of this object cannot move during the operation.
	m_parent->result_ptr = &m_result;

	return m_parent->operator co_await();
}

END_RUKEN_NAMESPACE