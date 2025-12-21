#include "Rendering/Resources/GPUImage.hpp"

USING_RUKEN_NAMESPACE

GPUImage::GPUImage(
	RenderDevice&			       in_device,
	vk::ImageCreateInfo     const& in_create_info,
	VmaAllocationCreateInfo const& in_alloc_info):
	device 	   {&in_device},
	image  	   {nullptr},
	allocation {}
{
	VkImage new_image;
	vmaCreateImage(device->GetAllocator(), &*in_create_info, &in_alloc_info, &new_image, &allocation, nullptr);
	image = new_image;
}

GPUImage::~GPUImage()
{
	vmaDestroyImage(device->GetAllocator(), image, allocation);
}
