#include "Rendering/Coroutines/GPUTask.hpp"

USING_RUKEN_NAMESPACE

GPUTask::GPUTask(GPUPromise& in_promise) noexcept:
	m_handle {std::addressof(in_promise)}
{
	MakeReference();
}

GPUTask::GPUTask(GPUTask const& in_other) noexcept:
	m_handle {in_other.m_handle}
{
	MakeReference();
}

GPUTask::GPUTask(GPUTask&& in_other) noexcept:
	m_handle {std::move(in_other.m_handle)}
{
	MakeReference();
}

GPUTask& GPUTask::operator=(GPUTask const& in_other) noexcept
{
	RemoveReference();
	m_handle = in_other.m_handle;
	MakeReference();

	return *this;
}

GPUTask& GPUTask::operator=(GPUTask&& in_other) noexcept
{
	RemoveReference();
	m_handle = in_other.m_handle;
	MakeReference();

	return *this;
}

GPUTask::~GPUTask() noexcept
{
	RemoveReference();
}

DynamicTask<> GPUTask::Submit(RenderDevice& in_device) noexcept
{
	GPUFence const fence {in_device.GetDevice(), {
		.flags = vk::FenceCreateFlagBits::eSignaled
	}};

	m_handle->Submit(in_device, fence);

	co_await fence;
}

RkVoid GPUTask::MakeReference() const noexcept
{
	if (m_handle)
		m_handle->references.fetch_add(1, std::memory_order_acq_rel);
}

RkVoid GPUTask::RemoveReference() const noexcept
{
	if (m_handle && m_handle->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
		std::coroutine_handle<GPUPromise>::from_promise(*m_handle).destroy();
}
