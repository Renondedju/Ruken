#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include "Rendering/Resources/Texture.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Texture::Texture(RenderDevice* in_device, std::string_view in_path) noexcept:
    m_device {in_device}
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

    m_image = std::make_unique<Image>(m_device, image_create_info);

    vk::ImageViewCreateInfo image_view_create_info = {
        .image = m_image->GetHandle(),
        .viewType = vk::ImageViewType::e2D,
        .format = vk::Format::eR8G8B8A8Unorm,
        .subresourceRange = {
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    if (auto [result, value] = m_device->GetLogicalDevice().createImageView(image_view_create_info); result == vk::Result::eSuccess)
    {
        m_image_view = value;
    }

    vk::SamplerCreateInfo tex_sampler_info = {
        .magFilter = vk::Filter::eLinear,
        .minFilter = vk::Filter::eLinear,
        .mipmapMode = vk::SamplerMipmapMode::eLinear,
        .borderColor = vk::BorderColor::eIntOpaqueBlack
    };

    if (auto [result, value] = m_device->GetLogicalDevice().createSampler(tex_sampler_info); result == vk::Result::eSuccess)
    {
        m_image_sampler = value;
    }

    vk::BufferCreateInfo buffer_create_info = {
        .size  = width * height * 4ULL,
        .usage = vk::BufferUsageFlagBits::eTransferSrc
    };

    Buffer staging_buffer(m_device, buffer_create_info);

    memcpy(staging_buffer.Map(), pixels, buffer_create_info.size);

    staging_buffer.UnMap();

    if (auto transfer_command_buffer = m_device->GetTransferQueue().BeginSingleUseCommandBuffer())
    {
        vk::ImageMemoryBarrier2KHR image_memory_barrier = {
            .srcStageMask        = vk::PipelineStageFlagBits2KHR::eNone,
            .srcAccessMask       = vk::AccessFlagBits2KHR::eNone,
            .dstStageMask        = vk::PipelineStageFlagBits2KHR::eTransfer,
            .dstAccessMask       = vk::AccessFlagBits2KHR::eMemoryWrite,
            .oldLayout           = vk::ImageLayout::eUndefined,
            .newLayout           = vk::ImageLayout::eTransferDstOptimal,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image               = m_image->GetHandle(),
            .subresourceRange =  {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1U,
                .layerCount = 1U
            }
        };

        vk::DependencyInfoKHR dependency_info = {
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers    = &image_memory_barrier
        };

        transfer_command_buffer.pipelineBarrier2KHR(dependency_info);

        vk::BufferImageCopy buffer_image_copy = {
            .imageSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            },
            .imageExtent = image_create_info.extent
        };

        transfer_command_buffer.copyBufferToImage(staging_buffer.GetHandle(), m_image->GetHandle(), vk::ImageLayout::eTransferDstOptimal, buffer_image_copy);

        image_memory_barrier.srcStageMask  = vk::PipelineStageFlagBits2KHR::eTransfer;
        image_memory_barrier.srcAccessMask = vk::AccessFlagBits2KHR::eMemoryWrite;
        image_memory_barrier.dstStageMask  = vk::PipelineStageFlagBits2KHR::eNone;
        image_memory_barrier.dstAccessMask = vk::AccessFlagBits2KHR::eNone;
        image_memory_barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
        image_memory_barrier.newLayout     = vk::ImageLayout::eReadOnlyOptimalKHR;

        if (m_device->HasDedicatedTransferQueue())
        {
            image_memory_barrier.srcQueueFamilyIndex = m_device->GetTransferFamilyIndex();
            image_memory_barrier.dstQueueFamilyIndex = m_device->GetGraphicsFamilyIndex();

            transfer_command_buffer.pipelineBarrier2KHR(dependency_info);

            m_device->GetTransferQueue().EndSingleUseCommandBuffer(transfer_command_buffer);

            if (auto graphics_command_buffer = m_device->GetGraphicsQueue().BeginSingleUseCommandBuffer())
            {
                image_memory_barrier.srcStageMask  = vk::PipelineStageFlagBits2KHR::eNone;
                image_memory_barrier.srcAccessMask = vk::AccessFlagBits2KHR::eNone;
                image_memory_barrier.dstStageMask  = vk::PipelineStageFlagBits2KHR::eFragmentShader;
                image_memory_barrier.dstAccessMask = vk::AccessFlagBits2KHR::eShaderRead;

                graphics_command_buffer.pipelineBarrier2KHR(dependency_info);

                m_device->GetGraphicsQueue().EndSingleUseCommandBuffer(graphics_command_buffer);
            }
        }

        else
        {
            image_memory_barrier.srcStageMask  = vk::PipelineStageFlagBits2KHR::eNone;
            image_memory_barrier.srcAccessMask = vk::AccessFlagBits2KHR::eNone;
            image_memory_barrier.dstStageMask  = vk::PipelineStageFlagBits2KHR::eFragmentShader;
            image_memory_barrier.dstAccessMask = vk::AccessFlagBits2KHR::eShaderRead;

            transfer_command_buffer.pipelineBarrier2KHR(dependency_info);

            m_device->GetTransferQueue().EndSingleUseCommandBuffer(transfer_command_buffer);
        }
    }

    stbi_image_free(pixels);
}

Texture::~Texture() noexcept
{
    m_device->GetLogicalDevice().destroyImageView(m_image_view);
    m_device->GetLogicalDevice().destroySampler  (m_image_sampler);
}

RkVoid Texture::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid Texture::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid Texture::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}

vk::Image const& Texture::GetImage() const noexcept
{
    return m_image->GetHandle();
}

vk::ImageView const& Texture::GetImageView() const noexcept
{
    return m_image_view;
}

vk::Sampler const& Texture::GetImageSampler() const noexcept
{
    return m_image_sampler;
}