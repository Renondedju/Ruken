#pragma once
#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"
#include "Core/JobSystem/Awaitables/Tasks/DynamicTask.hpp"

#include "Resources/ResourceData.hpp"

#include "Rendering/GPUWorkGraph.hpp"
#include "Rendering/ShaderModule.hpp"
#include "Rendering/Windowing/Window.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"

#include "Queues.hpp"

USING_RUKEN_NAMESPACE

struct SwapchainImage
{
	// This pointer needs to be kept alive for the image and view to stay valid.
	ResourcePtr<GPUSwapchainData> owner;

	vk::ImageView view;
	vk::Image     image;
};

/**
 * A render node, describes accesses and actual vulkan commands to execute.
 */
struct PaintTriangle
{
	vk::ImageView view;
	vk::Image	  image;
	vk::Pipeline  pipeline;
	vk::Viewport  viewport;
	vk::Extent2D  extent;

	/// @brief List of accesses. Needed for synchronisation.
	std::vector<GPUBufferAccess> buffer_accesses {};
	std::vector<GPUImageAccess > image_accesses  {
		GPUImageAccess {
			.image             = image,
			.layout            = vk::ImageLayout		   ::eColorAttachmentOptimal,
			.stages            = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			.access_flags      = vk::AccessFlagBits2       ::eColorAttachmentWrite,
			.subresource_range = {
				.aspectMask     = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel   = 0,
				.levelCount     = 1,
				.baseArrayLayer = 0,
				.layerCount     = 1
			}
		}
	};

	/// @brief Actual record command.
	RkVoid Record(vk::raii::CommandBuffer const& in_commands) const
	{
		vk::RenderingAttachmentInfo const attachment_info {
			.imageView   = view,
			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp      = vk::AttachmentLoadOp ::eClear,
			.storeOp     = vk::AttachmentStoreOp::eStore,
			.clearValue  = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
		};

		in_commands.beginRendering(vk::RenderingInfo {
			.renderArea = {
				.offset = { 0, 0 },
				.extent = extent
			},
			.layerCount           = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments    = &attachment_info
		});

		in_commands.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
		in_commands.setViewport (0, viewport);
		in_commands.setScissor  (0, vk::Rect2D(vk::Offset2D(0, 0), extent));
		in_commands.draw        (3, 1, 0, 0);
		in_commands.endRendering();

		vk::ImageMemoryBarrier2 const barrier2 {
			.srcStageMask   = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			.srcAccessMask  = vk::AccessFlagBits2       ::eColorAttachmentWrite,
			.dstStageMask   = vk::PipelineStageFlagBits2::eBottomOfPipe,
			.dstAccessMask  = {},
			.oldLayout      = vk::ImageLayout::eColorAttachmentOptimal,
			.newLayout      = vk::ImageLayout::ePresentSrcKHR,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image               = image,
			.subresourceRange    = {
				.aspectMask     = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel   = 0,
				.levelCount     = 1,
				.baseArrayLayer = 0,
				.layerCount     = 1
			}
		};

		in_commands.pipelineBarrier2(vk::DependencyInfo {
			.dependencyFlags         = {},
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers    = &barrier2
		});
	}
};

/**
 * A test renderer that presents something to a window.
 */
struct TestWindowRenderer
{
	RenderDevice&				 owner;
	Window&						 window;
	ResourceHandle<ShaderModule> pipeline;

	/**
	 * Renders a frame to a swapchain image.
	 * @return Async dynamic task.
	 */
	Task<ProcessingQueue> RenderFrame() const noexcept
	{
		// --- 1. Pre-configuration
		auto		 		const& pipeline_ptr      {pipeline.Current()};
		auto         		const& swapchain_ptr     {window.GetSwapchain().Current()};
		vk::raii::Semaphore const  acquire_semaphore {owner.GetDevice(), vk::SemaphoreCreateInfo()};
		vk::Extent2D	    const  extent            {window.GetExtent()};
		vk::Viewport	    const  viewport          {
			.x        = 0.0f, .y        = 0.0f,
			.width    = static_cast<float>(extent.width),
			.height   = static_cast<float>(extent.height),
			.minDepth = 0.0f, .maxDepth = 1.0f
		};

		// --- 2. Recording passes
		auto [result, image_index] = swapchain_ptr->swapchain.acquireNextImage(UINT64_MAX, *acquire_semaphore, nullptr);

		SwapchainImage const swapchain_image {
			.owner = swapchain_ptr,
			.view  = swapchain_ptr->images_views		 [image_index],
			.image = swapchain_ptr->swapchain.getImages()[image_index]
		};

		PaintTriangle painter {
			.view     = swapchain_image.view,
			.image    = swapchain_image.image,
			.pipeline = pipeline_ptr->pipeline,
			.viewport = viewport,
			.extent   = extent,
		};

		// TODO: Ideally all the synchro should be contained in the program
		GPUWorkGraph work_graph {owner};
		work_graph.AddPass(GPUWorkNode {
			.record_callback = Recordable(painter),
			.image_accesses  = painter.image_accesses,
			.buffer_accesses = painter.buffer_accesses
		});

		// --- 3. Waiting for execution to keep resources alive during execution.
		co_await work_graph.Submit(*acquire_semaphore, swapchain_ptr->present_semaphores[image_index]);

		std::ignore = owner.GetQueue().presentKHR(vk::PresentInfoKHR {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores    = &*swapchain_ptr->present_semaphores[image_index],
			.swapchainCount     = 1,
			.pSwapchains        = &*swapchain_ptr->swapchain,
			.pImageIndices      = &image_index,
			.pResults           = nullptr
		});

		co_return;
	};
};