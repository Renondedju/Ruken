#include "Rendering/RenderObjects/Image.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Image::Image(RenderDevice* in_device, vk::ImageCreateInfo const& in_image_create_info) noexcept:
    m_device {in_device}
{
    if (in_image_create_info.extent.width == 0ULL || in_image_create_info.extent.height == 0ULL)
        return;

    VmaAllocationCreateInfo allocation_create_info = {
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
    };
    
    vmaCreateImage(m_device->GetAllocator(), reinterpret_cast<VkImageCreateInfo const*>(&in_image_create_info), &allocation_create_info, reinterpret_cast<VkImage*>(&m_handle), &m_allocation, &m_allocation_info);
}

Image::~Image() noexcept
{
    vmaDestroyImage(m_device->GetAllocator(), m_handle, m_allocation);
}

vk::Image const& Image::GetHandle() const noexcept
{
    return m_handle;
}