#include "Rendering/GPUWorkGraph.hpp"

#include <map>
#include <ranges>
#include <set>

USING_RUKEN_NAMESPACE

GPUWorkGraph::GPUWorkGraph(RenderDevice& in_owner) noexcept:
	m_owner {in_owner}
{}

GPUWorkGraph::~GPUWorkGraph()
{}

RkVoid GPUWorkGraph::AddWorkNode(GPUWorkNode& in_node) noexcept
{
	m_nodes.emplace_back(std::addressof(in_node));
}

DynamicTask<> GPUWorkGraph::Submit(
	vk::Semaphore const in_wait_semaphore,
	vk::Semaphore const in_signal_semaphore)
{
	// --- 1. Listing all the required queue families
	std::set<SharedMutex<FamilyView>*> views {};
	for (auto const& work_node : m_nodes)
		if (auto family_view = m_owner.FindQueueFamily(work_node->queue_flags))
			views.emplace(family_view);

	// --- 2. Allocation of command buffers
	std::vector<SharedMutex<FamilyView>*> associated_view {m_nodes.size(), nullptr};
	std::vector<vk::raii::CommandBuffer>  command_buffers {};

	for (auto const& [work_node, view] : std::views::zip(m_nodes, associated_view))
	{
		view = m_owner.FindQueueFamily(work_node->queue_flags);
		if (!view)
			throw Exception("A GPUWorkGraph is trying to schedule a work node that is not supported by the target GPU.");

		auto const access {co_await view->AsyncRead()};

		command_buffers.emplace_back(std::move(m_owner.GetDevice().allocateCommandBuffers(vk::CommandBufferAllocateInfo {
			.commandPool 		= *access->command_pool,
			.level       		= vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = 1
		})[0]));
	}

	// --- 3. Allocation of semaphores
	std::vector<vk::raii::Semaphore>								semaphores		   {};
	std::map<vk::Image , std::tuple<GPUImageAccess , GPUWorkNode*>> last_image_access  {};
	std::map<vk::Buffer, std::tuple<GPUBufferAccess, GPUWorkNode*>> last_buffer_access {};

	for (auto const& [work_node, buffer] : std::views::zip(m_nodes, command_buffers))
	{
		for (GPUImageAccess const& access : work_node->image_accesses)
		{
			if (auto& last {last_image_access[access.image]}; std::get<0>(last).family_index != access.family_index)
			{
				auto& semaphore = semaphores.emplace_back(m_owner.GetDevice(), vk::SemaphoreCreateInfo {
					.flags = {}
				});

				work_node->waitSemaphores		   .emplace_back(vk::SemaphoreSubmitInfo {
					.semaphore = semaphore,
					.stageMask = access.stages
				});
				std::get<1>(last)->signalSemaphores.emplace_back(vk::SemaphoreSubmitInfo {
					.semaphore = semaphore,
					.stageMask = std::get<0>(last).stages
				});

				last = std::make_tuple(access, work_node);
			}
		}

		for (GPUBufferAccess const& access : work_node->buffer_accesses)
		{
			if (auto& last {last_buffer_access[access.buffer]}; std::get<0>(last).family_index != access.family_index)
			{
				auto& semaphore = semaphores.emplace_back(m_owner.GetDevice(), vk::SemaphoreCreateInfo {
					.flags = {}
				});

				work_node->waitSemaphores		   .emplace_back(vk::SemaphoreSubmitInfo {
					.semaphore = semaphore,
					.stageMask = access.stages
				});
				std::get<1>(last)->signalSemaphores.emplace_back(vk::SemaphoreSubmitInfo {
					.semaphore = semaphore,
					.stageMask = std::get<0>(last).stages
				});

				last = std::make_tuple(access, work_node);
			}
		}
	}

	if (in_wait_semaphore != nullptr)
		m_nodes.front()->waitSemaphores.emplace_back(vk::SemaphoreSubmitInfo {
			.semaphore = in_wait_semaphore,
			.stageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
		});

	if (in_signal_semaphore != nullptr)
		m_nodes.back()->signalSemaphores.emplace_back(vk::SemaphoreSubmitInfo {
			.semaphore = in_signal_semaphore,
			.stageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
		});

	// --- 4. Record & insert pipeline barriers
	std::map<vk::Image , GPUImageAccess>  barrier_image_access  {};
	std::map<vk::Buffer, GPUBufferAccess> barrier_buffer_access {};

	for (auto const& [work_node, buffer] : std::views::zip(m_nodes, command_buffers))
	{
		buffer.begin(vk::CommandBufferBeginInfo {
			.flags			  = {},
			.pInheritanceInfo = nullptr
		});

		// Image barriers
		for (GPUImageAccess const& access : work_node->image_accesses) {
			if (auto& last {barrier_image_access [access.image]};  last != access) {
				PipelineBarrier(buffer, last, access); last = access;
			}
		}

		// Buffer barriers
		for (GPUBufferAccess const& access : work_node->buffer_accesses) {
			if (auto& last {barrier_buffer_access[access.buffer]}; last != access) {
				PipelineBarrier(buffer, last, access); last = access;
			}
		}

		work_node->Record(buffer);
		buffer    .end();
	}

	// --- 5. Submit
	GPUFence const fence {m_owner.GetDevice(), vk::FenceCreateInfo {
		.flags = {}
	}};

	for (auto const& [work_node, view, buffer] : std::views::zip(m_nodes, associated_view, command_buffers))
	{
		auto access {co_await view->AsyncWrite()};

		vk::CommandBufferSubmitInfo submit_info {
			.commandBuffer = buffer,
			.deviceMask    = 0
		};

		access->queue->submit2(vk::SubmitInfo2 {
			.flags					  = {},
			.waitSemaphoreInfoCount   = static_cast<uint32_t>(work_node->waitSemaphores.size()),
			.pWaitSemaphoreInfos      = work_node->waitSemaphores.data(),
			.commandBufferInfoCount   = 1,
			.pCommandBufferInfos      = &submit_info,
			.signalSemaphoreInfoCount = static_cast<uint32_t>(work_node->signalSemaphores.size()),
			.pSignalSemaphoreInfos    = work_node->signalSemaphores.data()
		}, &work_node == &m_nodes.back() ? *fence.fence : nullptr);
	}

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
