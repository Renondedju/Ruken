#include "Swapchain.hpp"

USING_RUKEN_NAMESPACE

Swapchain::Swapchain(vk::raii::Device const& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info):
	Resource {},
	swapchain {in_device.createSwapchainKHR(in_swapchain_create_info)}
{}
