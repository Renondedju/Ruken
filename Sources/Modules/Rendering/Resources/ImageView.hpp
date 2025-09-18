#pragma once

#include "Resources/Resource.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUImageView final: Resource
{
	vk::raii::ImageView image_view;

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_owner Vulkan device.
	 * @param in_create_info Create info struct.
	 */
	GPUImageView(vk::raii::Device const& in_owner, vk::ImageViewCreateInfo const& in_create_info);
	GPUImageView(GPUImageView const&)			 = delete;
	GPUImageView(GPUImageView&&     )			 = default;
	GPUImageView& operator=(GPUImageView const&) = delete;
	GPUImageView& operator=(GPUImageView&&     ) = default;
	~GPUImageView() override					 = default;

	#pragma endregion
};

END_RUKEN_NAMESPACE