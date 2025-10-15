#include "GPUSwapchain.hpp"

USING_RUKEN_NAMESPACE

GPUSwapchainData::GPUSwapchainData(vk::raii::Device const& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info):
	ResourceData {},
	swapchain    {in_device.createSwapchainKHR(in_swapchain_create_info)},
	images_views {[&] { // -- Creating image views
		std::vector<vk::raii::ImageView> views {};

		for (auto const image : swapchain.getImages())
		{
			views.emplace_back(in_device, vk::ImageViewCreateInfo {
				.image            = image,
				.viewType         = vk::ImageViewType::e2D,
				.format           = in_swapchain_create_info.imageFormat,
				.components       = vk::ComponentMapping {
					.r = vk::ComponentSwizzle::eIdentity,
					.g = vk::ComponentSwizzle::eIdentity,
					.b = vk::ComponentSwizzle::eIdentity,
					.a = vk::ComponentSwizzle::eIdentity
				},
				.subresourceRange = vk::ImageSubresourceRange {
					.aspectMask     = vk::ImageAspectFlagBits::eColor,
					.baseMipLevel   = 0,
					.levelCount     = 1,
					.baseArrayLayer = 0,
					.layerCount     = 1
				}
			});
		}

		return views;
	}()},
	present_semaphores {[&] {
		std::vector<vk::raii::Semaphore> semaphores {};

		for (auto const _ : swapchain.getImages())
			semaphores.emplace_back(in_device, vk::SemaphoreCreateInfo { .flags = {}});

		return semaphores;
	}()}
{}
