#include "Rendering/Resources/GPUMesh.hpp"

#include <ranges>
#include <vk_mem_alloc.h>

USING_RUKEN_NAMESPACE

GPUMesh::GPUMesh(
	RenderDevice&			     in_device,
	std::vector<Vertex>   const& in_vertices,
	std::vector<RkUint32> const& in_indices):

	vertices {in_device, vk::BufferCreateInfo {
		.flags       = {},
		.size        = sizeof(in_vertices[0]) * in_vertices.size(),
		.usage       = vk::BufferUsageFlagBits::eVertexBuffer,
		.sharingMode = vk::SharingMode        ::eExclusive,
	}, VmaAllocationCreateInfo {
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	}},

	indices {in_device, vk::BufferCreateInfo {
		.flags       = {},
		.size        = sizeof(in_indices[0]) * in_indices.size(),
		.usage       = vk::BufferUsageFlagBits::eIndexBuffer,
		.sharingMode = vk::SharingMode		  ::eExclusive,
	}, VmaAllocationCreateInfo {
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	}}
{
	vmaCopyMemoryToAllocation(in_device.GetAllocator(), in_vertices.data(), vertices.allocation, 0, vertices.size);
	vmaCopyMemoryToAllocation(in_device.GetAllocator(), in_indices .data(), indices .allocation, 0, indices .size);
}

RkVoid GPUMesh::Draw(vk::raii::CommandBuffer const& in_command_buffer) const noexcept
{
	in_command_buffer.bindVertexBuffers(0, vertices.buffer, {0});
	in_command_buffer.bindIndexBuffer  (indices.buffer, 0, vk::IndexType::eUint32);
	in_command_buffer.drawIndexed	   (indices.size / sizeof(RkUint32), 1, 0, 0, 0);
}
