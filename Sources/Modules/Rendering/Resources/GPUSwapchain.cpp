#include "GPUSwapchain.hpp"
#include "RenderDevice.hpp"

USING_RUKEN_NAMESPACE

GPUSwapchainData::GPUSwapchainData(RenderDevice& in_device, vk::SwapchainCreateInfoKHR const& in_swapchain_create_info):
	IResourceData {},
	render_device {&in_device},
	swapchain     {in_device.GetDevice().createSwapchainKHR(in_swapchain_create_info)},
	images_views  {[&] { // -- Creating image views
		std::vector<vk::raii::ImageView> views {};

		for (auto const image : swapchain.getImages())
		{
			views.emplace_back(in_device.GetDevice(), vk::ImageViewCreateInfo {
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
	depth_image {in_device.GetDevice(), [&] {

		vk::ImageCreateInfo const depth_image_create_info {
			.imageType     = vk::ImageType::e2D,
			.format        = vk::Format::eD32Sfloat, // TODO: This value should be retrieved as a parameter instead
			.extent        {
				.width  = in_swapchain_create_info.imageExtent.width,
				.height = in_swapchain_create_info.imageExtent.height,
				.depth  = 1,
			},
			.mipLevels     = 1,
			.arrayLayers   = 1,
			.samples       = vk::SampleCountFlagBits::e1,
			.tiling        = vk::ImageTiling::eOptimal,
			.usage         = vk::ImageUsageFlagBits::eDepthStencilAttachment,
			.initialLayout = vk::ImageLayout::eUndefined,
		};

		VmaAllocationCreateInfo vma_allocation_create_info{
			.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
			.usage = VMA_MEMORY_USAGE_AUTO
		};

		VkImage image;
		vmaCreateImage(in_device.GetAllocator(), &*depth_image_create_info, &vma_allocation_create_info, &image, &depth_allocation, nullptr);
		return image;
	}()},
	depth_image_view {in_device.GetDevice().createImageView(vk::ImageViewCreateInfo {
		.image    = depth_image,
		.viewType = vk::ImageViewType::e2D,
		.format   = vk::Format::eD32Sfloat, // TODO: This value should be retrieved as a parameter instead
		.subresourceRange {
			.aspectMask = vk::ImageAspectFlagBits::eDepth,
			.levelCount = 1,
			.layerCount = 1
		}
	}, nullptr)}
{}
