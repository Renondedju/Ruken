#pragma once

#include "Resources/IResourceData.hpp"
#include "Rendering/RenderDevice.hpp"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

BEGIN_RUKEN_NAMESPACE

class GPUImage final: IResourceData
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_create_info Image create info.
	 */
	GPUImage(RenderDevice& in_device, vk::ImageCreateInfo const& in_create_info, VmaAllocationCreateInfo const& in_alloc_info);
	GPUImage(GPUImage const&)			 = delete;
	GPUImage(GPUImage&&     )			 = default;
	GPUImage& operator=(GPUImage const&) = delete;
	GPUImage& operator=(GPUImage&&     ) = default;
	~GPUImage() override;

	#pragma endregion

	#pragma region Members

	RenderDevice* device;
	vk::Image     image;
	VmaAllocation allocation;

	#pragma endregion
};

END_RUKEN_NAMESPACE