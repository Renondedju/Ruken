#include "Rendering/Resources/GPUBuffer.hpp"

USING_RUKEN_NAMESPACE

GPUBuffer::GPUBuffer(RenderDevice& in_device, vk::BufferCreateInfo const& in_create_info, VmaAllocationCreateInfo const& in_alloc_info):
	device      {&in_device},
	size        {in_create_info.size},
	buffer      {nullptr}
{
	VkBuffer init_buffer;
	vmaCreateBuffer(device->GetAllocator(), &*in_create_info, &in_alloc_info, &init_buffer, &allocation, nullptr);
	buffer		= init_buffer;
	buffer_info = vk::DescriptorBufferInfo {
		.buffer = buffer,
		.offset = 0,
		.range  = size
	};

	vmaMapMemory(device->GetAllocator(), allocation, &mapping);
}

GPUBuffer::~GPUBuffer()
{
	vmaUnmapMemory  (device->GetAllocator(), allocation);
	vmaDestroyBuffer(device->GetAllocator(), buffer, allocation);
}

RkVoid GPUBuffer::Update(RkSize const in_offset, RkSize const in_size, RkVoid const* in_data) const
{
	std::memcpy(static_cast<char*>(mapping) + in_offset, in_data, in_size);

	vmaFlushAllocation(device->GetAllocator(), allocation, in_offset, size);
}
