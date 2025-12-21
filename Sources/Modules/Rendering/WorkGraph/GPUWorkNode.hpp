#pragma once

#include "Rendering/WorkGraph/GPUImageAccess.hpp"
#include "Rendering/WorkGraph/GPUBufferAccess.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

/// @brief Describes accesses and actual vulkan commands to execute.
struct GPUWorkNode
{
	#pragma region Lifetime

	explicit GPUWorkNode(vk ::QueueFlags const			in_queue_flags,
						 std::vector<GPUImageAccess> && in_image_accesses,
						 std::vector<GPUBufferAccess>&& in_buffer_accesses) noexcept:
		queue_flags     {in_queue_flags},
		image_accesses  {std::move(in_image_accesses)},
		buffer_accesses {std::move(in_buffer_accesses)}
	{}

	GPUWorkNode(const GPUWorkNode&) 		   = default;
	GPUWorkNode(GPUWorkNode&&     ) 		   = default;
	GPUWorkNode& operator=(const GPUWorkNode&) = delete;
	GPUWorkNode& operator=(GPUWorkNode&&     ) = delete;
	virtual ~GPUWorkNode()					   = default;

	#pragma endregion

	#pragma region Members

	vk::QueueFlags const		 queue_flags;
	std::vector<GPUImageAccess>  image_accesses;
	std::vector<GPUBufferAccess> buffer_accesses;

	// TODO: Needs to be separated to keep the work node declarative
	std::vector<vk::SemaphoreSubmitInfo> waitSemaphores;
	std::vector<vk::SemaphoreSubmitInfo> signalSemaphores;

	#pragma endregion

	#pragma region Methods

	/// @brief Records a list of commands to be executed from the passed command buffer.
	virtual RkVoid Record(vk::raii::CommandBuffer const&) {}

	#pragma endregion
};

END_RUKEN_NAMESPACE