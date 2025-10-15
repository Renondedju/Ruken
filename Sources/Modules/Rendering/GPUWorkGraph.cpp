#include "Rendering/GPUWorkGraph.hpp"

#include <map>

USING_RUKEN_NAMESPACE

GPUWorkGraph::GPUWorkGraph(RenderDevice& in_owner) noexcept:
	m_owner {in_owner}
{}

GPUWorkGraph::~GPUWorkGraph()
{}

RkVoid GPUWorkGraph::AddPass(GPUWorkNode&& in_node) noexcept
{
	m_instructions.emplace_back(std::forward<GPUWorkNode>(in_node));
}

DynamicTask<> GPUWorkGraph::Submit(
	vk::Semaphore const in_wait_semaphore,
	vk::Semaphore const in_signal_semaphore) const noexcept
{
	vk::raii::Queue		    const& queue  {m_owner.GetQueue()};
	vk::raii::CommandBuffer const& buffer {m_owner.GetCommandBuffer()};

	// --- Synchronisation time
	std::map<vk::Image , GPUImageAccess>  last_image_access  {};
	std::map<vk::Buffer, GPUBufferAccess> last_buffer_access {};

	buffer.reset();
	buffer.begin(vk::CommandBufferBeginInfo {
		.flags			  = {},
		.pInheritanceInfo = nullptr
	});

	for (GPUWorkNode const& work_node : m_instructions)
	{
		// Image barriers
		for (GPUImageAccess const& access : work_node.image_accesses) {
			if (auto& last {last_image_access [access.image]};  last != access) {
				PipelineBarrier(buffer, last, access); last = access;
			}
		}

		// Buffer barriers
		for (GPUBufferAccess const& access : work_node.buffer_accesses) {
			if (auto& last {last_buffer_access[access.buffer]}; last != access) {
				PipelineBarrier(buffer, last, access); last = access;
			}
		}

		work_node.record_callback.Record(buffer);
	}

	buffer.end();

	// --- Submit time
	GPUFence fence {m_owner.GetDevice(), vk::FenceCreateInfo {
		.flags = {}
	}};

	static constexpr vk::PipelineStageFlags bottom {vk::PipelineStageFlagBits::eColorAttachmentOutput};

	queue.submit(vk::SubmitInfo {
		.waitSemaphoreCount   = 1,
		.pWaitSemaphores      = &in_wait_semaphore,
		.pWaitDstStageMask    = &bottom,
		.commandBufferCount   = 1,
		.pCommandBuffers      = &*buffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores    = &in_signal_semaphore,
	}, fence.fence);

	// Keeps synchronisation primitives alive for the duration of the execution.
	// Accessed resources can inherit this lifetime by waiting for this function.
	fence.WaitSynchronously();

	co_return;
}

RkVoid GPUWorkGraph::PipelineBarrier(
	vk::raii::CommandBuffer const& in_buffer,
	GPUImageAccess    	    const& in_from,
	GPUImageAccess    	    const& in_to) noexcept
{
	RUKEN_ASSERT(in_from.image == in_to.image || in_from.image == nullptr, "Cannot synchronize an image into another.");

	vk::ImageMemoryBarrier2 const barrier {
		.srcStageMask   	 = in_from.stages,
		.srcAccessMask  	 = in_from.access_flags,
		.dstStageMask   	 = in_to  .stages,
		.dstAccessMask  	 = in_to  .access_flags,
		.oldLayout      	 = in_from.layout,
		.newLayout      	 = in_to  .layout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image               = in_to  .image,
		.subresourceRange    = in_to  .subresource_range, // TODO: Subresource must be compared
	};

	in_buffer.pipelineBarrier2(vk::DependencyInfo {
		.dependencyFlags         = {},
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers    = &barrier
	});
}

RkVoid GPUWorkGraph::PipelineBarrier(
	vk::raii::CommandBuffer const& in_buffer,
	GPUBufferAccess         const& in_from,
	GPUBufferAccess         const& in_to) noexcept
{
	RUKEN_ASSERT(in_from.buffer == in_to.buffer || in_to.buffer == nullptr, "Cannot synchronize a buffer into another.");

	vk::BufferMemoryBarrier2 const barrier {
		.srcStageMask   	 = in_from.stages,
		.srcAccessMask  	 = in_from.access_flags,
		.dstStageMask   	 = in_to  .stages,
		.dstAccessMask  	 = in_to  .access_flags,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.buffer              = in_to  .buffer,
		.offset				 = in_to  .offset,
		.size				 = in_to  .size, // TODO: offset & size must be compared
	};

	in_buffer.pipelineBarrier2(vk::DependencyInfo {
		.dependencyFlags          = {},
		.bufferMemoryBarrierCount = 1,
		.pBufferMemoryBarriers    = &barrier
	});
}
