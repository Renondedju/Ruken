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
        .tiling      = vk::ImageTiling::eOptimal,
        .usage       = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst
    };

    vk::AllocationCreateInfo allocation_create_info = {
        .usage = vk::MemoryUsage::eGpuOnly
    };

    auto [result, value] = m_device->GetAllocator().createImage(image_create_info, allocation_create_info, m_allocation_info);

    std::tie(m_image, m_allocation) = value;

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

    vk::BufferCreateInfo buffer_create_info = {
        .size  = width * height * 4ULL,
        .usage = vk::BufferUsageFlagBits::eTransferSrc
    };

    allocation_create_info.flags = vk::AllocationCreateFlagBits::eMapped;
    allocation_create_info.usage = vk::MemoryUsage::eCpuOnly;

    vk::AllocationInfo allocation_info;

    std::pair<vk::Buffer, vk::Allocation> staging_buffer;

    std::tie(result, staging_buffer) = m_device->GetAllocator().createBuffer(buffer_create_info, allocation_create_info, allocation_info);

    memcpy(allocation_info.pMappedData, pixels, allocation_info.size);

    if (m_device->HasDedicatedTransferQueue())
    {
        auto transfer_command_buffer = m_device->GetTransferQueue().AcquireSingleUseCommandBuffer();

        vk::ImageMemoryBarrier barrier = {
            .srcAccessMask    = vk::AccessFlagBits::eNoneKHR,
            .dstAccessMask    = vk::AccessFlagBits::eTransferWrite,
            .oldLayout        = vk::ImageLayout::eUndefined,
            .newLayout        = vk::ImageLayout::eTransferDstOptimal,
            .image            = m_image,
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        transfer_command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);

        vk::BufferImageCopy buffer_image_copy = {
            .imageSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            },
            .imageExtent = image_create_info.extent
        };

        transfer_command_buffer.copyBufferToImage(staging_buffer.first, m_image, vk::ImageLayout::eTransferDstOptimal, buffer_image_copy);

        barrier.srcAccessMask       = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask       = vk::AccessFlagBits::eNoneKHR;
        barrier.oldLayout           = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout           = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcQueueFamilyIndex = m_device->GetTransferFamilyIndex();
        barrier.dstQueueFamilyIndex = m_device->GetGraphicsFamilyIndex();
        
        transfer_command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);

        m_device->GetTransferQueue().ReleaseSingleUseCommandBuffer(transfer_command_buffer);

        barrier.srcAccessMask       = vk::AccessFlagBits::eNoneKHR;
        barrier.dstAccessMask       = vk::AccessFlagBits::eShaderRead;
        barrier.oldLayout           = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout           = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcQueueFamilyIndex = m_device->GetTransferFamilyIndex();
        barrier.dstQueueFamilyIndex = m_device->GetGraphicsFamilyIndex();

        auto graphics_command_buffer = m_device->GetGraphicsQueue().AcquireSingleUseCommandBuffer();

        graphics_command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);

        m_device->GetGraphicsQueue().ReleaseSingleUseCommandBuffer(graphics_command_buffer);
    }

    else
    {
        auto command_buffer = m_device->GetTransferQueue().AcquireSingleUseCommandBuffer();

        vk::ImageMemoryBarrier barrier = {
            .srcAccessMask = vk::AccessFlagBits::eNoneKHR,
            .dstAccessMask = vk::AccessFlagBits::eTransferWrite,
            .oldLayout = vk::ImageLayout::eUndefined,
            .newLayout = vk::ImageLayout::eTransferDstOptimal,
            .image = m_image,
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);

        vk::BufferImageCopy buffer_image_copy = {
            .imageSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            },
            .imageExtent = image_create_info.extent
        };

        command_buffer.copyBufferToImage(staging_buffer.first, m_image, vk::ImageLayout::eTransferDstOptimal, buffer_image_copy);

        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
        barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout     = vk::ImageLayout::eShaderReadOnlyOptimal;

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);

        m_device->GetTransferQueue().ReleaseSingleUseCommandBuffer(command_buffer);
    }

    m_device->GetAllocator().destroyBuffer(staging_buffer.first, staging_buffer.second);

    stbi_image_free(pixels);
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