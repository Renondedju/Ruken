#pragma once

#include "Rendering/WorkGraph/GPUBufferAccess.hpp"
#include "Rendering/WorkGraph/GPUImageAccess.hpp"
#include "Rendering/WorkGraph/GPUWorkNode.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/GpuFence.hpp"

#include <vulkan/vulkan.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUWorkGraph
{
	#pragma region Lifetime

	explicit GPUWorkGraph(RenderDevice& in_owner) noexcept;
	GPUWorkGraph		   (GPUWorkGraph&&)		 = delete;
	GPUWorkGraph		   (const GPUWorkGraph&) = delete;
	GPUWorkGraph& operator=(GPUWorkGraph&&)	     = delete;
	GPUWorkGraph& operator=(const GPUWorkGraph&) = delete;
	~GPUWorkGraph();

	#pragma endregion

	#pragma region Methods

	RkVoid AddWorkNode(GPUWorkNode& in_node) noexcept;

	DynamicTask<> Submit(vk::Semaphore in_wait_semaphore, vk::Semaphore in_signal_semaphore);

	static RkVoid PipelineBarrier(vk::raii::CommandBuffer const& in_buffer, GPUImageAccess  const& in_from, GPUImageAccess  const& in_to) noexcept;
	static RkVoid PipelineBarrier(vk::raii::CommandBuffer const& in_buffer, GPUBufferAccess const& in_from, GPUBufferAccess const& in_to) noexcept;

	#pragma endregion

	private:

		#pragma region Members

		RenderDevice&			  m_owner;
		std::vector<GPUWorkNode*> m_nodes;

		#pragma endregion
};

END_RUKEN_NAMESPACE