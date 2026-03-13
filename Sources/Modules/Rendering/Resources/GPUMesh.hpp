#pragma once

#include "Rendering/Resources/GPUBuffer.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Vertex.hpp"

BEGIN_RUKEN_NAMESPACE

struct GPUMesh final: IResourceData
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_device Owning device.
	 * @param in_vertices List of vertices.
	 * @param in_indices List of indices.
	 */
	explicit GPUMesh(
		RenderDevice&			     in_device,
		std::vector<Vertex>   const& in_vertices,
		std::vector<RkUint32> const& in_indices
	);
	GPUMesh			  (GPUMesh const&) = delete;
	GPUMesh			  (GPUMesh&&)      = default;
	GPUMesh& operator=(GPUMesh const&) = delete;
	GPUMesh& operator=(GPUMesh&&)      = default;
	~GPUMesh() override                = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * Binds the vertex and index buffers to the passed command buffer.
	 * @param in_command_buffer Command buffer to bind to.
	 * @param in_instance_count Number of instances to render
	 */
	RkVoid Draw(vk::raii::CommandBuffer const& in_command_buffer, RkUint32 in_instance_count = 1) const noexcept;

	#pragma endregion

	#pragma region Members

	GPUBuffer vertices;
	GPUBuffer indices;

	#pragma endregion
};

END_RUKEN_NAMESPACE