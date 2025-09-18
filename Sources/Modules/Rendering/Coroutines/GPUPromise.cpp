#include "Rendering/Coroutines/GPUPromise.hpp"
#include "Rendering/Coroutines/GPUTask.hpp"

#include <coroutine>

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

GPUPromise::InitialSuspend GPUPromise::initial_suspend() noexcept
{
	return InitialSuspend {
		.promise = this
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
		.flags			  = vk::CommandBufferUsageFlagBits::eOneTimeSubmit, // TODO: ?
		.pInheritanceInfo = nullptr
	});
}

bool GPUPromise::FinalSuspend::await_ready() noexcept
{
	return false;
}

void GPUPromise::FinalSuspend::await_suspend(std::coroutine_handle<>) noexcept
{
	promise->command_buffer.end();

	// Destroys the coroutine in place if there are no more references.
	// What about submission ?
	// - Done in place means : one time submit flag, no keeping recorded buffers around, consistent with a regular CPU task.
	// - Manual submit means : Manual submit can be confusing. More performant if done well (optim).  Command buffer alloc ?
	/* return promise->references.fetch_sub(1UZ, std::memory_order_acq_rel) == 1UZ*/;

	// TODO: - Read from graph
	//		 - Wait semaphores
	//       - Signal semaphores
	// in_device.GetQueue().submit(vk::SubmitInfo {
	// 	.waitSemaphoreCount   = 1,
	// 	.pWaitSemaphores      = &*present_complete_semaphore,
	// 	.pWaitDstStageMask    = &wait_destination_stage_mask,
	// 	.commandBufferCount   = 1,
	// 	.pCommandBuffers      = &*command_buffer,
	// 	.signalSemaphoreCount = 1,
	// 	.pSignalSemaphores    = &*render_finished_semaphore
	// }, in_fence.fence);

	// co_await fence;
}

void GPUPromise::FinalSuspend::await_resume() noexcept
{

}

#pragma endregion
