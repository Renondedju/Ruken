#pragma once

#include "Rendering/RenderDevice.hpp"
#include "Resources/IResourceData.hpp"

#include <vulkan/vulkan.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUBuffer final: IResourceData
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_create_info Buffer create info.
	 * @param in_alloc_info Allocation info.
	 */
	explicit GPUBuffer(
		RenderDevice&				   in_device,
		vk::BufferCreateInfo    const& in_create_info,
		VmaAllocationCreateInfo const& in_alloc_info
	);

	GPUBuffer(GPUBuffer const&)			   = delete;
	GPUBuffer(GPUBuffer&&     )			   = default;
	GPUBuffer& operator=(GPUBuffer const&) = delete;
	GPUBuffer& operator=(GPUBuffer&&     ) = default;
	~GPUBuffer() override;

	#pragma endregion

	#pragma region Members

	RenderDevice*  device;
	vk::DeviceSize size;
	vk::Buffer     buffer;
	VmaAllocation  allocation;

	#pragma endregion
};

END_RUKEN_NAMESPACE