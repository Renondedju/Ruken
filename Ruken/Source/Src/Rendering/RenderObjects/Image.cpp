#include "Rendering/RenderObjects/Image.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Image::Image(RenderDevice* in_device) noexcept:
    m_device {in_device}
{
    
}

Image::Image(RenderDevice* in_device, vk::Extent3D const& in_extent, vk::Image const& in_handle, vk::ImageView const& in_view) noexcept:
    m_device {in_device},
    m_extent {in_extent},
    m_handle {in_handle},
    m_view   {in_view}
{
    
}

Image::Image(RenderDevice* in_device, vk::ImageCreateInfo const& in_image_create_info) noexcept:
    m_device {in_device},
    m_extent {in_image_create_info.extent}
{
    CreateImage(in_image_create_info);

    vk::ImageViewCreateInfo image_view_create_info = {
        .image = m_handle,
        .viewType = vk::ImageViewType::e2D,
        .format = in_image_create_info.format,
        .subresourceRange = {
            .aspectMask = in_image_create_info.usage & vk::ImageUsageFlagBits::eColorAttachment ? vk::ImageAspectFlagBits::eColor : vk::ImageAspectFlagBits::eDepth,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    CreateView(image_view_create_info);
}

Image::~Image() noexcept
{
    if (m_allocation)
        vmaDestroyImage(m_device->GetAllocator(), m_handle, m_allocation);
    if (m_view)
        m_device->GetLogicalDevice().destroy(m_view);
}

RkVoid Image::CreateImage(vk::ImageCreateInfo const& in_image_create_info) noexcept
{
    if (in_image_create_info.extent.width == 0ULL || in_image_create_info.extent.height == 0ULL)
        return;

    VmaAllocationCreateInfo allocation_create_info = {
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
    };
    
    vmaCreateImage(m_device->GetAllocator(),
                   reinterpret_cast<VkImageCreateInfo const*>(&in_image_create_info),
                   &allocation_create_info, reinterpret_cast<VkImage*>(&m_handle),
                   &m_allocation,
                   &m_allocation_info);
}

RkVoid Image::CreateView(vk::ImageViewCreateInfo const& in_view_create_info) noexcept
{
    m_view = m_device->GetLogicalDevice().createImageView(in_view_create_info).value;
}

vk::Extent3D const& Image::GetExtent() const noexcept
{
    return m_extent;
}

vk::Image const& Image::GetHandle() const noexcept
{
    return m_handle;
}

vk::ImageView const& Image::GetView() const noexcept
{
    return m_view;
}