#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include "Rendering/Resources/Texture.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Texture::Texture(RenderDevice* in_device, std::string_view in_path) noexcept: DeviceObjectBase(in_device)
{
    int32_t width, height, channels;

    stbi_uc* pixels = stbi_load(in_path.data(), &width, &height, &channels, STBI_rgb_alpha);

    vk::ImageCreateInfo image_create_info = {
        .imageType = vk::ImageType::e2D,
        .format    = vk::Format::eR8G8B8A8Unorm,
        .extent    = {
            .width  = static_cast<uint32_t>(width),
            .height = static_cast<uint32_t>(height),
            .depth  = 1
        },
        .mipLevels   = 1,
        .arrayLayers = 1,
        .tiling      = vk::ImageTiling::eLinear,
        .usage       = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst
    };

    vk::AllocationCreateInfo allocation_create_info = {
        .flags = vk::AllocationCreateFlagBits::eMapped,
        .usage = vk::MemoryUsage::eCpuToGpu
    };

    auto [result, value] = m_device->GetAllocator().createImage(image_create_info, allocation_create_info, m_allocation_info);

    std::tie(m_image, m_allocation) = value;

    memcpy(m_allocation_info.pMappedData, pixels, m_allocation_info.size);

    vk::ImageViewCreateInfo image_view_create_info = {
        .image = m_image,
        .viewType = vk::ImageViewType::e2D,
        .format = vk::Format::eR8G8B8A8Unorm,
        .subresourceRange = {
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    std::tie(result, m_image_view) = m_device->GetLogicalDevice().createImageView(image_view_create_info);

    vk::SamplerCreateInfo tex_sampler_info = {
        .magFilter = vk::Filter::eLinear,
        .minFilter = vk::Filter::eLinear,
        .mipmapMode = vk::SamplerMipmapMode::eLinear,
        .borderColor = vk::BorderColor::eIntOpaqueBlack
    };

    std::tie(result, m_image_sampler) = m_device->GetLogicalDevice().createSampler(tex_sampler_info);
}

Texture::~Texture() noexcept
{
    m_device->GetAllocator    ().destroyImage    (m_image, m_allocation);
    m_device->GetLogicalDevice().destroyImageView(m_image_view);
    m_device->GetLogicalDevice().destroySampler  (m_image_sampler);
}

vk::Image const& Texture::GetImage() const noexcept
{
    return m_image;
}

vk::ImageView const& Texture::GetImageView() const noexcept
{
    return m_image_view;
}

vk::Sampler const& Texture::GetImageSampler() const noexcept
{
    return m_image_sampler;
}