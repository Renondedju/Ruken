#pragma once

#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"
#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"

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

struct PreparePresentation final : GPUWorkNode
{
	explicit PreparePresentation(vk::Image const in_presentation_image) noexcept:
		GPUWorkNode {
			vk::QueueFlagBits::eGraphics, {
				GPUImageAccess {
					.image             = in_presentation_image,
					.layout            = vk::ImageLayout::ePresentSrcKHR,
					.stages            = vk::PipelineStageFlagBits2::eBottomOfPipe,
					.access_flags      = {},
					.subresource_range = {
						.aspectMask     = vk::ImageAspectFlagBits::eColor,
						.baseMipLevel   = 0,
						.levelCount     = 1,
						.baseArrayLayer = 0,
						.layerCount     = 1
					},
				}
			}, {}
		}
	{}

	RkVoid Record(vk::raii::CommandBuffer const& in_command_buffer) override
	{
		// Presentation is not a command
	}
};

/// @brief Draws a simple triangle to the screen
struct PaintTriangle final : GPUWorkNode
{
	#pragma region Lifetime

	PaintTriangle(
		vk::ImageView const  in_view,
		vk::Image	  const  in_image,
		vk::Pipeline  const  in_pipeline,
		vk::Viewport  const& in_viewport,
		vk::Extent2D  const  in_extent
	) noexcept:
		GPUWorkNode {
			vk::QueueFlagBits::eGraphics, {
			GPUImageAccess {
				.image             = in_image,
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
			}}, {}
		},
		view        {in_view},
		image       {in_image},
		pipeline    {in_pipeline},
		viewport    {in_viewport},
		extent      {in_extent}
	{}

	PaintTriangle(const PaintTriangle&) 		   = default;
	PaintTriangle(PaintTriangle&&     ) 		   = default;
	PaintTriangle& operator=(const PaintTriangle&) = delete;
	PaintTriangle& operator=(PaintTriangle&&     ) = delete;
	~PaintTriangle() override					   = default;

	#pragma endregion

	vk::ImageView view;
	vk::Image	  image;
	vk::Pipeline  pipeline;
	vk::Viewport  viewport;
	vk::Extent2D  extent;

	/// @brief Actual record command.
	RkVoid Record(vk::raii::CommandBuffer const& in_commands) override
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

		// TODO: Ideally all the synchro should be contained in the program
		GPUWorkGraph work_graph {owner};

		PaintTriangle draw_triangle {
			swapchain_image.view,
			swapchain_image.image,
			pipeline_ptr->pipeline,
			viewport,
			extent,
		};
		PreparePresentation presentation {
			swapchain_image.image
		};

		work_graph.AddWorkNode(draw_triangle);
		work_graph.AddWorkNode(presentation);

		// --- 3. Waiting for execution to keep resources alive during execution.
		co_await work_graph.Submit(*acquire_semaphore, swapchain_ptr->present_semaphores[image_index]);

		GPUFence present_fence {owner.GetDevice(), vk::FenceCreateInfo()};

		vk::StructureChain present_chain {
			vk::PresentInfoKHR {
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &*swapchain_ptr->present_semaphores[image_index],
				.swapchainCount     = 1,
				.pSwapchains        = &*swapchain_ptr->swapchain,
				.pImageIndices      = &image_index,
				.pResults           = nullptr
			},
			vk::SwapchainPresentFenceInfoEXT {
				.swapchainCount = 1,
				.pFences		= &*present_fence.fence
			}
		};

		auto graphics_queue {co_await owner.FindQueueFamily(vk::QueueFlagBits::eGraphics)->AsyncWrite()};
		std::ignore = graphics_queue->queue->presentKHR(present_chain.get<>());

		present_fence.WaitSynchronously();

		co_return;
	};
};