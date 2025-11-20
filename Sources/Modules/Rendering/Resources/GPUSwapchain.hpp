#pragma once

#include "Resources/ResourceData.hpp"
#include "Resources/ResourceHandle.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUSwapchainData final : ResourceData
{
	vk::raii::SwapchainKHR			 swapchain;
	std::vector<vk::raii::ImageView> images_views;

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_swapchain_create_info Swapchain create info.
	 */
	GPUSwapchainData(vk::raii::Device const& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info);
	GPUSwapchainData(GPUSwapchainData const&)			 = delete;
	GPUSwapchainData(GPUSwapchainData&&     )			 = default;
	GPUSwapchainData& operator=(GPUSwapchainData const&) = delete;
	GPUSwapchainData& operator=(GPUSwapchainData&&     ) = default;
	~GPUSwapchainData() override						 = default;

	#pragma endregion
};

using GPUSwapchain = ResourceHandle<GPUSwapchainData>;

END_RUKEN_NAMESPACE