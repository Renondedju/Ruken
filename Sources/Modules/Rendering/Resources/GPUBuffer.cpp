#include "Rendering/Resources/GPUBuffer.hpp"

USING_RUKEN_NAMESPACE

GPUBuffer::GPUBuffer(RenderDevice& in_device, vk::BufferCreateInfo const& in_create_info, VmaAllocationCreateInfo const& in_alloc_info):
	device     {&in_device},
	size       {in_create_info.size},
	buffer     {nullptr},
	allocation {}
{
	VkBuffer init_buffer;
	vmaCreateBuffer(device->GetAllocator(), &*in_create_info, &in_alloc_info, &init_buffer, &allocation, nullptr);
	buffer = init_buffer;
}

GPUBuffer::~GPUBuffer()
{
	vmaDestroyBuffer(device->GetAllocator(), buffer, allocation);
}
