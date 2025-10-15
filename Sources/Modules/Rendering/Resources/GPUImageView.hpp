#pragma once

#include "Resources/ResourceData.hpp"
#include "Resources/ResourceHandle.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUImageViewData final: ResourceData
{
	vk::raii::ImageView image_view;

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_owner Vulkan device.
	 * @param in_create_info Create info struct.
	 */
	GPUImageViewData(vk::raii::Device const& in_owner, vk::ImageViewCreateInfo const& in_create_info);
	GPUImageViewData(GPUImageViewData const&)			 = delete;
	GPUImageViewData(GPUImageViewData&&     )			 = default;
	GPUImageViewData& operator=(GPUImageViewData const&) = delete;
	GPUImageViewData& operator=(GPUImageViewData&&     ) = default;
	~GPUImageViewData() override					     = default;

	#pragma endregion
};

using GPUImageView = ResourceHandle<GPUImageViewData>;

END_RUKEN_NAMESPACE