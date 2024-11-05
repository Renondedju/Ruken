#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTaskPromise.hpp"

BEGIN_RUKEN_NAMESPACE

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTaskPromise<TQueueHandle, TResult>& in_parent, CPUContinuationNodePtr& in_continuation_node) noexcept:
	CPUAwaitable<TPromiseAwaitableValue<TResult>> {in_continuation_node, std::addressof(in_parent.value)},
	m_parent			                          {std::addressof(in_parent)},
	m_coroutine_handle                            {std::coroutine_handle<CPUTaskPromise<TQueueHandle, TResult>>::from_promise(in_parent)}
{
	m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTask const& in_other) noexcept:
	CPUAwaitable<TPromiseAwaitableValue<TResult>> {in_other},
	m_parent		   							  {in_other.m_parent},
	m_coroutine_handle 							  {in_other.m_coroutine_handle}
{
	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>::CPUTask(CPUTask&& in_other) noexcept:
	CPUAwaitable<TPromiseAwaitableValue<TResult>> {std::move(in_other)},
	m_parent		   							  {std::move(in_other.m_parent)},
	m_coroutine_handle 							  {std::move(in_other.m_coroutine_handle)}
{
	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);
}

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>::~CPUTask() noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();
}

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>& CPUTask<TQueueHandle, TResult>::operator=(CPUTask const& in_other) noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();

	CPUAwaitable<TPromiseAwaitableValue<TResult>>::operator=(in_other);
	m_parent		   = in_other.m_parent;
	m_coroutine_handle = in_other.m_coroutine_handle;

	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

template<CQueueHandle TQueueHandle, typename TResult>
CPUTask<TQueueHandle, TResult>& CPUTask<TQueueHandle, TResult>::operator=(CPUTask&& in_other) noexcept
{
	if (m_parent && m_parent->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_coroutine_handle.destroy();

	CPUAwaitable<TPromiseAwaitableValue<TResult>>::operator=(std::move(in_other));
	m_parent		   = std::move(in_other.m_parent);
	m_coroutine_handle = std::move(in_other.m_coroutine_handle);

	if (m_parent)
		m_parent->m_references.fetch_add(1, std::memory_order_acq_rel);

	return *this;
}

END_RUKEN_NAMESPACE