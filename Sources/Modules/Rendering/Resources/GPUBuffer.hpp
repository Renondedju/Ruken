#pragma once

#include "Resources/Resource.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

class GPUBuffer final: Resource
{
	vk::raii::Buffer buffer;

	// Synchronisation tracking
	vk::PipelineStageFlagBits2 current_stage_flag;
	vk::AccessFlagBits2        current_access_flag;
	RkUint32				   current_queue_family {VK_QUEUE_FAMILY_IGNORED};

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_create_info Buffer create info.
	 */
	GPUBuffer(vk::raii::Device const& in_device, vk::BufferCreateInfo const& in_create_info);
	GPUBuffer(GPUBuffer const&)			   = delete;
	GPUBuffer(GPUBuffer&&     )			   = default;
	GPUBuffer& operator=(GPUBuffer const&) = delete;
	GPUBuffer& operator=(GPUBuffer&&     ) = default;
	~GPUBuffer() override				   = default;

	#pragma endregion
};

END_RUKEN_NAMESPACE