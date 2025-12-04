#pragma once

#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"
#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Core/Maths/Vector/DistanceVector3.hpp"

#include "Rendering/Windowing/Window.hpp"
#include "Rendering/WorkGraph/GPUWorkGraph.hpp"
#include "Rendering/Resources/ShaderModule.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"
#include "Rendering/Resources/GPUMesh.hpp"

#include "Resources/IResourceData.hpp"

#include "Queues.hpp"

#include <tiny_obj_loader.h>

#include "Maths/Matrix/Matrix.hpp"
#include "Types/Units/Duration/Duration.hpp"

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
struct DrawMesh final : GPUWorkNode
{
	#pragma region Lifetime

	DrawMesh(
		vk::ImageView      const  in_view,
		vk::Image	       const  in_image,
		vk::Pipeline       const  in_pipeline,
		vk::PipelineLayout const  in_layout,
		ResourcePtr<GPUMesh>      in_mesh,
		vk::Viewport       const& in_viewport,
		vk::Extent2D       const  in_extent,
		vk::DescriptorSet  const  in_set
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
		view     {in_view},
		image    {in_image},
		pipeline {in_pipeline},
		layout   {in_layout},
		mesh     {in_mesh},
		viewport {in_viewport},
		extent   {in_extent},
		set	     {in_set}
	{}

	DrawMesh(const DrawMesh&) 		     = default;
	DrawMesh(DrawMesh&&     ) 		     = default;
	DrawMesh& operator=(const DrawMesh&) = delete;
	DrawMesh& operator=(DrawMesh&&     ) = delete;
	~DrawMesh() override				 = default;

	#pragma endregion

	vk::ImageView        view;
	vk::Image	         image;
	vk::Pipeline         pipeline;
	vk::PipelineLayout   layout;
	ResourcePtr<GPUMesh> mesh;
	vk::Viewport         viewport;
	vk::Extent2D         extent;
	vk::DescriptorSet    set;

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
		in_commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, set, nullptr);

		mesh->Draw(in_commands);

		in_commands.endRendering();
	}
};

struct UniformBufferObject
{
	alignas(16) Matrix<4, 4> model;
	alignas(16) Matrix<4, 4> view;
	alignas(16) Matrix<4, 4> projection;
};

/**
 * A test renderer that presents something to a window.
 */
struct TestWindowRenderer
{
	RenderDevice&				 owner;
	Window&						 window;
	ResourceHandle<ShaderModule> pipeline;
	ResourceHandle<GPUMesh>		 mesh;

	vk::DescriptorPoolSize   pool_size		 {vk::DescriptorType::eUniformBuffer, 10};
	vk::raii::DescriptorPool descriptor_pool {owner.GetDevice(), vk::DescriptorPoolCreateInfo {
		.flags         = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
		.maxSets       = pool_size.descriptorCount,
		.poolSizeCount = 1,
		.pPoolSizes    = &pool_size
	}};
	std::vector<vk::DescriptorSetLayout> layouts {pool_size.descriptorCount, *pipeline.Current()->descriptor_set_layout};
	vk::raii::DescriptorSets descriptor_sets {owner.GetDevice(), vk::DescriptorSetAllocateInfo {
		.descriptorPool		= descriptor_pool,
		.descriptorSetCount = static_cast<RkUint32>(layouts.size()), // Dependency to the pipeline !
								// Needs to be stored in shader module with a maximum amount of simultaneous invocations
		.pSetLayouts		= layouts.data()
	}};

	std::atomic<RkSize> frame_index {0};

	/**
	 * Renders a frame to a swapchain image.
	 * @return Async dynamic task.
	 */
	Task<ProcessingQueue> RenderFrame(Seconds in_time) noexcept
	{
		auto const& pipeline_ptr  {pipeline				.Current()};
		auto const& swapchain_ptr {window.GetSwapchain().Current()};
		++frame_index;

		// --- 1 Uniform buffer
		vk::DeviceSize ubo_size {sizeof(UniformBufferObject)};
		GPUBuffer      ubo      {owner, vk::BufferCreateInfo {
			.flags                 = {},
			.size                  = ubo_size,
			.usage                 = vk::BufferUsageFlagBits::eUniformBuffer,
			.sharingMode           = vk::SharingMode::eExclusive,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices   = nullptr
		}, VmaAllocationCreateInfo {
			.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			.usage = VMA_MEMORY_USAGE_AUTO
		}};

		UniformBufferObject const ubo_data {
			.model 	    = Matrix4x4::ModelMatrix(
				{0_m, 0_m, 10_m},
				{0_deg, static_cast<Degrees>(static_cast<RkFloat>(in_time * 40.0f)), 0_deg},
				Constants<Vector3m>::one / 2
			),
			.view  	    = {},
			.projection = Matrix4x4::OrthogonalProjectionMatrix(-1_m, 1_m, -1_m, 1_m, 1_cm, 1_km) * Matrix4x4::ClipSpace()
		};

		vmaCopyMemoryToAllocation(ubo.device->GetAllocator(), &ubo_data, ubo.allocation, 0, sizeof(UniformBufferObject));

		vk::DescriptorBufferInfo buffer_info {
			.buffer = ubo.buffer,
			.offset = 0,
			.range  = ubo_size
		};
		vk::WriteDescriptorSet write_descriptors {
			.dstSet           = descriptor_sets[frame_index % swapchain_ptr->images_views.size()],
			.dstBinding       = 0,
			.dstArrayElement  = 0,
			.descriptorCount  = 1,
			.descriptorType   = vk::DescriptorType::eUniformBuffer,
			.pBufferInfo      = &buffer_info,
		};

		owner.GetDevice().updateDescriptorSets(write_descriptors, {});

		// --- 1.2 Pipeline & swapchain setup
		vk::raii::Semaphore const  acquire_semaphore {owner .GetDevice(), vk::SemaphoreCreateInfo()};
		vk::raii::Semaphore const  submit_semaphore  {owner .GetDevice(), vk::SemaphoreCreateInfo()};
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

		DrawMesh draw_triangle {
			swapchain_image.view,
			swapchain_image.image,
			pipeline_ptr->pipeline,
			pipeline_ptr->pipeline_layout,
			mesh.Current(),
			viewport,
			extent,
			descriptor_sets[frame_index % swapchain_ptr->images_views.size()]
		};
		PreparePresentation presentation {
			swapchain_image.image
		};

		work_graph.AddWorkNode(draw_triangle);
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