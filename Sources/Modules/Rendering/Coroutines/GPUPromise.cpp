#include "Rendering/Coroutines/GPUPromise.hpp"
#include "Rendering/Coroutines/GPUTask.hpp"

#include <coroutine>

#include "Coroutines/TracyVkUtilities.hpp"

USING_RUKEN_NAMESPACE

void GPUPromise::unhandled_exception() noexcept
{
	// TODO
	std::terminate();
}

GPUTask GPUPromise::get_return_object() noexcept
{
	return GPUTask(*this);
}

void GPUPromise::await_transform(GPUTask const& in_task) noexcept
{
	// TODO
}

#pragma region Coroutine Methods

GPUPromise::InitialSuspend GPUPromise::initial_suspend(std::source_location const in_source_location) noexcept
{
	return InitialSuspend {
		.promise			= this,
		.coroutine_location = in_source_location
	};
}

GPUPromise::FinalSuspend GPUPromise::final_suspend() noexcept
{
	return FinalSuspend {
		.promise = this
	};
}

void GPUPromise::return_void() noexcept
{}

RkBool GPUPromise::SignalConsume() const noexcept
{
	TracyVkCollectHost(device.TracyContext())

	return ManualResetEvent::SignalConsume();
}

bool GPUPromise::InitialSuspend::await_ready() noexcept
{
	return true;
}

void GPUPromise::InitialSuspend::await_suspend(std::coroutine_handle<>) noexcept
{}

void GPUPromise::InitialSuspend::await_resume() noexcept
{
	GPU::command_buffer   = std::addressof(promise->command_buffer);
	promise->command_buffer.begin(vk::CommandBufferBeginInfo {
		.flags			  = {},//vk::CommandBufferUsageFlagBits::,
		.pInheritanceInfo = nullptr
	});

	promise->tracy_vk_scope = TracyVkUtilities::TracyVulkanZone(promise->device.TracyContext(),
		*promise->command_buffer, coroutine_location, true
	);
}

bool GPUPromise::FinalSuspend::await_ready() noexcept
{
	return false;
}

bool GPUPromise::FinalSuspend::await_suspend(std::coroutine_handle<> const) noexcept
{
	// Tracy makes use of RAII to properly write timestamps
	// but since the coroutine is kept alive until the end of GPU execution
	// we need to manually call the destructor here.
	promise->tracy_vk_scope.reset();

	// Submission is done in place: one time submit flag,
	// no keeping recorded buffers around, consistent with a regular CPU task.
	promise->command_buffer.end();
	promise->device		   .GetQueue().submit(vk::SubmitInfo {
		.waitSemaphoreCount   = 0,
		.pWaitSemaphores      = nullptr,
		.pWaitDstStageMask    = nullptr,
		.commandBufferCount   = 1,
		.pCommandBuffers      = &*promise->command_buffer,
		.signalSemaphoreCount = 0,
		.pSignalSemaphores    = nullptr
	}, promise->end_execution_fence);

	CheckFenceStatus(promise, GPUTask(*promise));

	// Destroys the coroutine in place if there are no more references.
	return promise->references.fetch_sub(1UZ, std::memory_order_acq_rel) != 1UZ;
}

void GPUPromise::FinalSuspend::await_resume() noexcept
{}

DynamicTask<> GPUPromise::FinalSuspend::CheckFenceStatus(GPUPromise* in_promise, GPUTask const in_task) noexcept
{
	vk::Result const status {in_promise->end_execution_fence.getStatus()};

	if (status == vk::Result::eSuccess ) auto _ {in_promise->SignalConsume()};   // Signaling CPU awaiters
	if (status == vk::Result::eNotReady) CheckFenceStatus(in_promise, in_task); // Or try again later ...

	// Others values such as eDeviceLost are ignored and the loop is ended.
	co_return;
}

#pragma endregion
