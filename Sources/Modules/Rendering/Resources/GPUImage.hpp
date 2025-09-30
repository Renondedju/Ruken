#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Resources/ResourceData.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

class GPUImage final: ResourceData
{
	vk::raii::Image	image;

	// Synchronisation tracking
	vk::PipelineStageFlagBits2 current_stage_flag;
	vk::AccessFlagBits2        current_access_flag;
	vk::ImageLayout            current_image_layout;
	RkUint32				   current_queue_family {VK_QUEUE_FAMILY_IGNORED};

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_create_info Image create info.
	 */
	GPUImage(vk::raii::Device const& in_device, vk::ImageCreateInfo const& in_create_info);
	GPUImage(GPUImage const&)			 = delete;
	GPUImage(GPUImage&&     )			 = default;
	GPUImage& operator=(GPUImage const&) = delete;
	GPUImage& operator=(GPUImage&&     ) = default;
	~GPUImage() override				 = default;

	#pragma endregion
};

END_RUKEN_NAMESPACE