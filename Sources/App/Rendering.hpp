#pragma once

#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Core/Maths/Vector/DistanceVector.hpp"

#include "Rendering/Windowing/Window.hpp"
#include "Rendering/WorkGraph/GPUWorkGraph.hpp"
#include "Rendering/Resources/ShaderModule.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"
#include "Rendering/Resources/GPUMesh.hpp"

#include "Resources/IResourceData.hpp"

#include <tiny_obj_loader.h>

#include "ECSTask.hpp"
#include "Coroutines/TracyVkUtilities.hpp"
#include "Maths/Matrix/Matrix.hpp"
#include "Shaders/TransformBuffer.hpp"
#include "Simulation/Flock.hpp"
#include "Types/Units/Duration/Duration.hpp"

USING_RUKEN_NAMESPACE

struct SwapchainImage
{
	// This pointer needs to be kept alive for the image and view to stay valid.
	ResourcePtr<GPUSwapchainData> owner;

	vk::ImageView view;
	vk::Image     image;
	vk::ImageView depth_view;
	vk::Image	  depth_image;
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
	{ /* Presentation is not a command */ }
};

/// @brief Draws a simple mesh to the screen
struct DrawMesh final : GPUWorkNode
{
	#pragma region Lifetime

	DrawMesh(
		vk::ImageView        const  in_framebuffer_view,
		vk::ImageView	     const  in_depth_view,
		vk::Image	         const  in_framebuffer_image,
		vk::Image            const  in_depth_image,
		vk::Pipeline         const  in_pipeline,
		vk::PipelineLayout   const  in_layout,
		ResourcePtr<GPUMesh> const& in_mesh,
		vk::Viewport         const& in_viewport,
		vk::Extent2D         const  in_extent,
		std::vector<vk::DescriptorSet> const& in_sets
	) noexcept:
		GPUWorkNode {
			vk::QueueFlagBits::eGraphics, {
			GPUImageAccess {
				.image             = in_framebuffer_image,
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
			},
			GPUImageAccess {
				.image             = in_depth_image,
				.layout            = vk::ImageLayout		   ::eDepthAttachmentOptimal,
				.stages            = vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
				.access_flags      = vk::AccessFlagBits2       ::eDepthStencilAttachmentWrite,
				.subresource_range = {
					.aspectMask     = vk::ImageAspectFlagBits::eDepth,
					.baseMipLevel   = 0,
					.levelCount     = 1,
					.baseArrayLayer = 0,
					.layerCount     = 1
				}
			}}, {}
		},
		framebuffer_view {in_framebuffer_view},
		depth_view       {in_depth_view},
		pipeline 		 {in_pipeline},
		layout   		 {in_layout},
		mesh     		 {in_mesh},
		viewport 		 {in_viewport},
		extent   		 {in_extent},
		sets	     	 {in_sets}
	{}

	DrawMesh(const DrawMesh&) 		     = default;
	DrawMesh(DrawMesh&&     ) 		     = default;
	DrawMesh& operator=(const DrawMesh&) = delete;
	DrawMesh& operator=(DrawMesh&&     ) = delete;
	~DrawMesh() override				 = default;

	#pragma endregion

	vk::ImageView        framebuffer_view;
	vk::ImageView        depth_view;

	vk::Pipeline         pipeline;
	vk::PipelineLayout   layout;
	ResourcePtr<GPUMesh> mesh;
	vk::Viewport         viewport;
	vk::Extent2D         extent;
	std::vector<vk::DescriptorSet> sets;

	/// @brief Actual record command.
	RkVoid Record(vk::raii::CommandBuffer const& in_commands) override
	{
		vk::RenderingAttachmentInfo const color_attachment_info {
			.imageView   = framebuffer_view,
			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp      = vk::AttachmentLoadOp ::eClear,
			.storeOp     = vk::AttachmentStoreOp::eStore,
			.clearValue  = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
		};

		vk::RenderingAttachmentInfo const depth_attachment_info {
			.imageView   = depth_view,
			.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
			.loadOp      = vk::AttachmentLoadOp ::eClear,
			.storeOp     = vk::AttachmentStoreOp::eStore,
			.clearValue  = vk::ClearDepthStencilValue(1.0f, 0),
		};

		in_commands.beginRendering(vk::RenderingInfo {
			.renderArea = {
				.offset = { 0, 0 },
				.extent = extent
			},
			.layerCount           = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments    = &color_attachment_info,
			.pDepthAttachment	  = &depth_attachment_info,
		});

		in_commands.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
		in_commands.setViewport (0, viewport);
		in_commands.setScissor  (0, vk::Rect2D(vk::Offset2D(0, 0), extent));
		in_commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, sets, nullptr);

		mesh->Draw(in_commands, 10'000);

		in_commands.endRendering();
	}
};

/// @brief Draws a simple mesh to the screen
struct DrawGrid final : GPUWorkNode
{
	#pragma region Lifetime

	DrawGrid(
		vk::ImageView        const  in_framebuffer_view,
		vk::ImageView	     const  in_depth_view,
		vk::Image	         const  in_framebuffer_image,
		vk::Image            const  in_depth_image,
		vk::Pipeline         const  in_pipeline,
		vk::PipelineLayout   const  in_layout,
		vk::Viewport         const& in_viewport,
		vk::Extent2D         const  in_extent,
		std::vector<vk::DescriptorSet> const& in_sets
	) noexcept:
		GPUWorkNode {
			vk::QueueFlagBits::eGraphics, {
			GPUImageAccess {
				.image             = in_framebuffer_image,
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
			},
			GPUImageAccess {
				.image             = in_depth_image,
				.layout            = vk::ImageLayout		   ::eDepthAttachmentOptimal,
				.stages            = vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
				.access_flags      = vk::AccessFlagBits2       ::eDepthStencilAttachmentRead,
				.subresource_range = {
					.aspectMask     = vk::ImageAspectFlagBits::eDepth,
					.baseMipLevel   = 0,
					.levelCount     = 1,
					.baseArrayLayer = 0,
					.layerCount     = 1
				}
			}}, {}
		},
		framebuffer_view {in_framebuffer_view},
		depth_view       {in_depth_view},
		pipeline 		 {in_pipeline},
		layout   		 {in_layout},
		viewport 		 {in_viewport},
		extent   		 {in_extent},
		sets	     	 {in_sets}
	{}

	DrawGrid(const DrawGrid&) 		     = default;
	DrawGrid(DrawGrid&&     ) 		     = default;
	DrawGrid& operator=(const DrawGrid&) = delete;
	DrawGrid& operator=(DrawGrid&&     ) = delete;
	~DrawGrid() override				 = default;

	#pragma endregion

	vk::ImageView        framebuffer_view;
	vk::ImageView        depth_view;
	vk::Pipeline         pipeline;
	vk::PipelineLayout   layout;
	vk::Viewport         viewport;
	vk::Extent2D         extent;
	std::vector<vk::DescriptorSet> sets;

	/// @brief Actual record command.
	RkVoid Record(vk::raii::CommandBuffer const& in_commands) override
	{
		vk::RenderingAttachmentInfo const color_attachment_info {
			.imageView   = framebuffer_view,
			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp      = vk::AttachmentLoadOp ::eLoad,
			.storeOp     = vk::AttachmentStoreOp::eStore,
		};

		vk::RenderingAttachmentInfo const depth_attachment_info {
			.imageView   = depth_view,
			.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
			.loadOp      = vk::AttachmentLoadOp ::eLoad,
			.storeOp     = vk::AttachmentStoreOp::eDontCare,
		};

		in_commands.beginRendering(vk::RenderingInfo {
			.renderArea = {
				.offset = { 0, 0 },
				.extent = extent
			},
			.layerCount           = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments    = &color_attachment_info,
			.pDepthAttachment	  = &depth_attachment_info,
		});

		in_commands.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
		in_commands.setViewport (0, viewport);
		in_commands.setScissor  (0, vk::Rect2D(vk::Offset2D(0, 0), extent));
		in_commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, sets, nullptr);
		in_commands.draw(6, 1, 0, 0);

		in_commands.endRendering();
	}
};

/**
 * A test renderer that presents something to a window.
 */
struct TestWindowRenderer
{
	RenderDevice&           owner;
	Window&		            window;
	SpirvCode&              boid_code;
	SpirvCode&              grid_code;
	ResourceHandle<GPUMesh> mesh;
	CameraController&		controller;
	Flock&					flock;

	std::array<vk::DescriptorPoolSize, 2> pool_sizes {
		vk::DescriptorPoolSize {vk::DescriptorType::eUniformBuffer, 10},
		vk::DescriptorPoolSize {vk::DescriptorType::eStorageBuffer, 10}
	};

	vk::raii::DescriptorPool descriptor_pool {owner.GetDevice(), vk::DescriptorPoolCreateInfo {
		.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
		.maxSets       = 10,
		.poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
		.pPoolSizes    = pool_sizes.data()
	}};

	TransformBufferStorage transform_buffer_storage {owner, descriptor_pool, flock.BoidCount(), 3};
	GridParametersStorage  grid_parameters_storage  {owner, descriptor_pool, 3};
	CameraDataStorage      camera_data_storage	    {owner, descriptor_pool, 3};
	ShaderModule	       boid_shader 				{owner, boid_code, std::vector{*transform_buffer_storage.layout, *camera_data_storage.layout}};
	ShaderModule	       grid_shader 				{owner, grid_code, std::vector{*grid_parameters_storage .layout, *camera_data_storage.layout}};
	std::atomic<RkSize>    frame_index			    {0};

	/**
	 * Renders a frame to a swapchain image.
	 * @return Async dynamic task.
	 */
	AsyncTask<ECSJobQueue> RenderFrame(Seconds in_time) noexcept
	{
		auto const& swapchain_ptr {window.GetSwapchain().Current()};
		auto const  buffer_index  {frame_index % swapchain_ptr->images_views.size()};
		auto const  size          {window.GetSize()};
		auto const  extent        {vk::Extent2D {
			.width  = static_cast<RkUint32>(size.Width ()),
			.height = static_cast<RkUint32>(size.Height()),
		}};

		++frame_index;

		// --- 1 Uniform buffers
		transform_buffer_storage.Update(flock.GetTransforms(), buffer_index);
		grid_parameters_storage .Update(GridParameters { }	 , buffer_index);
		camera_data_storage	    .Update(controller.Update()  , buffer_index);

		// --- 1.2 Pipeline & swapchain setup
		vk::raii::Semaphore const  acquire_semaphore {owner .GetDevice(), vk::SemaphoreCreateInfo()};
		vk::raii::Semaphore const  submit_semaphore  {owner .GetDevice(), vk::SemaphoreCreateInfo()};
		vk::Viewport	    const  viewport          {
			.x        = 0.0f, .y        = 0.0f,
			.width    = static_cast<float>(size.Width ()),
			.height   = static_cast<float>(size.Height()),
			.minDepth = 0.0f, .maxDepth = 1.0f
		};

		// --- 2. Recording passes
		auto [result, image_index] = swapchain_ptr->swapchain.acquireNextImage(UINT64_MAX, *acquire_semaphore, nullptr);

		SwapchainImage const swapchain_image {
			.owner       = swapchain_ptr,
			.view        = swapchain_ptr->images_views		   [image_index],
			.image       = swapchain_ptr->swapchain.getImages()[image_index],
			.depth_view  = swapchain_ptr->depth_image_view,
			.depth_image = swapchain_ptr->depth_image,
		};

		// TODO: Ideally all the synchro should be contained in the program
		GPUWorkGraph work_graph {owner};

		DrawMesh draw_triangle {
			swapchain_image.view,
			swapchain_image.depth_view,
			swapchain_image.image,
			swapchain_image.depth_image,
			boid_shader.pipeline,
			boid_shader.pipeline_layout,
			mesh.Current(),
			viewport,
			extent,
			{
				transform_buffer_storage.sets[buffer_index],
				camera_data_storage		.sets[buffer_index],
			}
		};
		DrawGrid draw_grid {
			swapchain_image.view,
			swapchain_image.depth_view,
			swapchain_image.image,
			swapchain_image.depth_image,
			grid_shader.pipeline,
			grid_shader.pipeline_layout,
			viewport,
			extent,
			{
				grid_parameters_storage.sets[buffer_index],
				camera_data_storage	   .sets[buffer_index],
			}
		};
		PreparePresentation presentation {
			swapchain_image.image
		};

		work_graph.AddWorkNode(draw_triangle);
		work_graph.AddWorkNode(draw_grid);
		work_graph.AddWorkNode(presentation);

		// --- 3. Waiting for execution to keep resources alive during execution.
		co_await work_graph.Submit(*acquire_semaphore, submit_semaphore);

		GPUFence		   present_fence   {owner.GetDevice(), vk::FenceCreateInfo()};
		vk::StructureChain structure_chain {
			vk::PresentInfoKHR {
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &*submit_semaphore,
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
		std::ignore = graphics_queue->queue->presentKHR(structure_chain.get<>());

		present_fence.WaitSynchronously();

		co_return;
	}
};