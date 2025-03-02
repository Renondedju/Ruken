#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, CTaskResult TResult>
auto CPUTask<TQueueHandle, TResult>::operator co_await() const noexcept
{
	return m_parent->operator co_await();
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTaskPromise<TQueueHandle, TResult>& in_parent) noexcept:
	m_parent		   {std::addressof(in_parent)},
	m_coroutine_handle {std::coroutine_handle<CPUTaskPromise<TQueueHandle, TResult>>::from_promise(in_parent)}
{
	m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTask const& in_other) noexcept:
	m_parent		   {in_other.m_parent},
	m_coroutine_handle {in_other.m_coroutine_handle}
{
	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTask&& in_other) noexcept:
	m_parent		   {std::move(in_other.m_parent)},
	m_coroutine_handle {std::move(in_other.m_coroutine_handle)}
{
	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>::~CPUTask() noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>& CPUTask<TQueueHandle, TResult>::operator=(CPUTask const& in_other) noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();

	m_parent		   = in_other.m_parent;
	m_coroutine_handle = in_other.m_coroutine_handle;

	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

template<CQueueHandle TQueueHandle, CTaskResult TResult>
CPUTask<TQueueHandle, TResult>& CPUTask<TQueueHandle, TResult>::operator=(CPUTask&& in_other) noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();

	m_parent		   = std::move(in_other.m_parent);
	m_coroutine_handle = std::move(in_other.m_coroutine_handle);

	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

END_RUKEN_NAMESPACE