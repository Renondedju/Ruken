#include "Rendering/RenderTarget.hpp"
#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

RenderTarget::RenderTarget(RenderDevice* in_device, RkUint32 in_width, RkUint32 in_height, vk::Format in_format, vk::ImageUsageFlags in_usage) noexcept:
    m_device {in_device},
    m_extent {
        .width  = in_width,
        .height = in_height,
        .depth  = 1U 
    }
{
    vk::ImageCreateInfo image_create_info = {
        .imageType   = vk::ImageType::e2D,
        .format      = in_format,
        .extent      = m_extent,
        .mipLevels   = 1,
        .arrayLayers = 1,
        .tiling      = vk::ImageTiling::eOptimal,
        .usage       = in_usage
    };

    vk::AllocationCreateInfo allocation_create_info = {
        .usage = vk::MemoryUsage::eGpuOnly
    };

    auto [result, value] = m_device->GetAllocator().createImage(image_create_info, allocation_create_info);

    std::tie(m_image, m_allocation) = value;

    vk::ImageViewCreateInfo image_view_create_info = {
        .image = m_image,
        .viewType = vk::ImageViewType::e2D,
        .format = in_format,
        .subresourceRange = {
            .aspectMask = in_usage & vk::ImageUsageFlagBits::eColorAttachment ? vk::ImageAspectFlagBits::eColor : vk::ImageAspectFlagBits::eDepth,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    std::tie(result, m_image_view) = m_device->GetLogicalDevice().createImageView(image_view_create_info);
}

RenderTarget::~RenderTarget() noexcept
{
    m_device->GetAllocator().destroyImage(m_image, m_allocation);

    m_device->GetLogicalDevice().destroy(m_image_view);
}

vk::Extent3D const& RenderTarget::GetExtent() const noexcept
{
    return m_extent;
}

vk::Image const& RenderTarget::GetImage() const noexcept
{
    return m_image;
}

vk::ImageView const& RenderTarget::GetImageView() const noexcept
{
    return m_image_view;
}