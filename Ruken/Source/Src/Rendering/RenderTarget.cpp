#include "Rendering/RenderTarget.hpp"
#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

RenderTarget::RenderTarget(RenderDevice* in_device, AttachmentInfo const& in_attachment_info) noexcept:
    m_device {in_device},
    m_extent {in_attachment_info.extent}
{
    vk::ImageCreateInfo image_create_info = {
        .imageType   = vk::ImageType::e2D,
        .format      = in_attachment_info.format,
        .extent      = m_extent,
        .mipLevels   = 1,
        .arrayLayers = 1,
        .tiling      = vk::ImageTiling::eOptimal,
        .usage       = in_attachment_info.usage
    };

    m_image = std::make_unique<Image>(m_device, image_create_info);

    vk::ImageViewCreateInfo image_view_create_info = {
        .image = m_image->GetHandle(),
        .viewType = vk::ImageViewType::e2D,
        .format = in_attachment_info.format,
        .subresourceRange = {
            .aspectMask = in_attachment_info.usage & vk::ImageUsageFlagBits::eColorAttachment ? vk::ImageAspectFlagBits::eColor : vk::ImageAspectFlagBits::eDepth,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    if (auto [result, value] = m_device->GetLogicalDevice().createImageView(image_view_create_info); result == vk::Result::eSuccess)
    {
        m_image_view = value;
    }
}

RenderTarget::~RenderTarget() noexcept
{
    m_device->GetLogicalDevice().destroy(m_image_view);
}

vk::Extent3D const& RenderTarget::GetExtent() const noexcept
{
    return m_extent;
}

vk::Image const& RenderTarget::GetImage() const noexcept
{
    return m_image->GetHandle();
}

vk::ImageView const& RenderTarget::GetImageView() const noexcept
{
    return m_image_view;
}