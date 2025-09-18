#pragma once

#include "Resources/Resource.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct Swapchain final : Resource
{
	vk::raii::SwapchainKHR swapchain;

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_swapchain_create_info Swapchain create info.
	 */
	Swapchain(vk::raii::Device const& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info);
	Swapchain(Swapchain const&)			   = delete;
	Swapchain(Swapchain&&     )			   = default;
	Swapchain& operator=(Swapchain const&) = delete;
	Swapchain& operator=(Swapchain&&     ) = default;
	~Swapchain() override				   = default;

	#pragma endregion
};

END_RUKEN_NAMESPACE