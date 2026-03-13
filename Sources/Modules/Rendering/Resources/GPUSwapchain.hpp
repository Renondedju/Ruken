#pragma once

#include "Rendering/RenderDevice.hpp"

#include "Resources/IResourceData.hpp"
#include "Resources/ResourceHandle.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.h>

BEGIN_RUKEN_NAMESPACE

struct GPUSwapchainData final : IResourceData
{
	RenderDevice* render_device;

	// Color framebuffer
	vk::raii::SwapchainKHR			 swapchain;
	std::vector<vk::raii::ImageView> images_views;

	// Depth Attachment
	vk::Image			depth_image;
	vk::raii::ImageView	depth_image_view;
	VmaAllocation       depth_allocation;

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_swapchain_create_info Swapchain create info.
	 */
	GPUSwapchainData(RenderDevice& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info);
	GPUSwapchainData(GPUSwapchainData const&)			 = delete;
	GPUSwapchainData(GPUSwapchainData&&     )			 = default;
	GPUSwapchainData& operator=(GPUSwapchainData const&) = delete;
	GPUSwapchainData& operator=(GPUSwapchainData&&     ) = default;
	~GPUSwapchainData() override;

	#pragma endregion
};

using GPUSwapchain = ResourceHandle<GPUSwapchainData>;

END_RUKEN_NAMESPACE