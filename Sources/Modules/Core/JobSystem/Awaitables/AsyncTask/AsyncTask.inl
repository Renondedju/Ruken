#pragma once

#include "JobSystem/Awaitables/AsyncTask/AsyncTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE
template<IsQueueHandle TQueueHandle, typename TResult>
RkBool AsyncTask<TQueueHandle, TResult>::Done() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return m_handle.done();
}

template<IsQueueHandle TQueueHandle, typename TResult>
auto AsyncTask<TQueueHandle, TResult>::operator co_await() const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	return m_parent->operator co_await();
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>::AsyncTask(AsyncTaskPromise<TQueueHandle, TResult>& in_parent) noexcept:
	m_parent {std::addressof(in_parent)},
	m_handle {std::coroutine_handle<AsyncTaskPromise<TQueueHandle, TResult>>::from_promise(in_parent)}
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	m_parent->references.fetch_add(1, std::memory_order_acq_rel);
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>::AsyncTask(AsyncTask const& in_other) noexcept:
	m_parent {in_other.m_parent},
	m_handle {in_other.m_handle}
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_parent)
		m_parent->references.fetch_add(1, std::memory_order_acq_rel);
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>::AsyncTask(AsyncTask&& in_other) noexcept:
	m_parent {std::move(in_other.m_parent)},
	m_handle {std::move(in_other.m_handle)}
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_parent)
		m_parent->references.fetch_add(1, std::memory_order_acq_rel);
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>::~AsyncTask() noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_parent && m_parent->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_handle.destroy();
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>& AsyncTask<TQueueHandle, TResult>::operator=(AsyncTask const& in_other) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_parent && m_parent->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_handle.destroy();

	m_parent = in_other.m_parent;
	m_handle = in_other.m_handle;

	if (m_parent)
		m_parent->references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

template<IsQueueHandle TQueueHandle, typename TResult>
AsyncTask<TQueueHandle, TResult>& AsyncTask<TQueueHandle, TResult>::operator=(AsyncTask&& in_other) noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_PROMISE_ZONES));

	if (m_parent && m_parent->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_handle.destroy();

	m_parent = std::move(in_other.m_parent);
	m_handle = std::move(in_other.m_handle);

	if (m_parent)
		m_parent->references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

END_RUKEN_NAMESPACE