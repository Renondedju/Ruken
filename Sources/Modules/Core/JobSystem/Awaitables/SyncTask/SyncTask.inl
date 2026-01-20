#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTaskPromise<TResult>& in_parent) noexcept:
	m_handle {std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(in_parent)},
	m_parent {std::addressof(in_parent)}
{}

template<typename TResult>
SyncTask<TResult>::~SyncTask()
{
	// If coroutine is not done before destruction, that means that is has not been co_awaited.
	// Therefore, it must be run to completion before destruction of the handle for safety.
	// This also allows to start a synchronous task inside regular functions.

	if (m_parent && !m_handle.done())
		m_handle.resume();
}

template<typename TResult>
SyncTaskAwaiter<TResult> SyncTask<TResult>::operator co_await() const noexcept
{
	// Address of this object cannot move during the operation.
	m_parent->result_ptr = &m_result;

	return m_parent->operator co_await();
}

END_RUKEN_NAMESPACE