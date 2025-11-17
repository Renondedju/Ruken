#include "Rendering/GpuFence.hpp"

USING_RUKEN_NAMESPACE

GPUFence::GPUFence(vk::raii::Device const& in_device, vk::FenceCreateInfo const& in_fence_create_info):
	ManualResetEvent {},
	fence            {in_device, in_fence_create_info},
	device			 {in_device}
{}

DynamicTask<> GPUFence::CheckFenceStatus(/*std::stop_token		in_stop_token,*/
										 vk::raii::Fence const& in_fence,
										 GPUFence		 const& in_event) noexcept
{
	/*if (in_stop_token.stop_requested())
		co_return;
	*/
	vk::Result const status {in_fence.getStatus()};

	if (status == vk::Result::eSuccess)
		in_event.SignalConsume(); // Signaling awaiters

	if (status == vk::Result::eNotReady)
		CheckFenceStatus(in_fence, in_event);  // Try again later ...

	// Others values such as eDeviceLost are ignored and the loop is ended.
	co_return;
}

RkVoid GPUFence::Reset() const noexcept
{
	ManualResetEvent::Reset();
	device.resetFences(*fence);
}

Awaiter GPUFence::operator co_await() const noexcept
{
	//TODO: Call only once
	CheckFenceStatus(fence, *this);

	return ManualResetEvent::operator co_await();
}

RkVoid GPUFence::WaitSynchronously() const noexcept
{
	while (device.waitForFences(*fence, true, UINT64_MAX) != vk::Result::eSuccess)
		;
}
