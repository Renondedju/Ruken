#pragma once

#include "Core/JobSystem/Awaitables/SyncTask/SyncTask.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TResult>
SyncTask<TResult>::SyncTask(SyncTaskPromise<TResult>& in_parent) noexcept:
	m_parent {std::addressof(in_parent)},
	m_handle {std::coroutine_handle<SyncTaskPromise<TResult>>::from_promise(in_parent)}
{}

template<typename TResult>
SyncTaskAwaiter<TResult> SyncTask<TResult>::operator co_await() const noexcept
{
	return m_parent->operator co_await();
}

END_RUKEN_NAMESPACE