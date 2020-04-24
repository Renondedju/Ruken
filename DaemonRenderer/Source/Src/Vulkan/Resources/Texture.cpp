/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma warning (push, 0)

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#pragma warning (pop)

#include "Vulkan/Resources/Texture.hpp"

#include "Rendering/RenderSystem.hpp"

#include "Resource/ResourceProcessingFailure.hpp"

#include "Vulkan/CommandPool.hpp"

#include "Vulkan/Core/VulkanFence.hpp"
#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanQueue.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

std::optional<VulkanImage> Texture::CreateImage(DAEuint32 const in_width, DAEuint32 const in_height) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkImageCreateInfo image_create_info = {};

    image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType     = VK_IMAGE_TYPE_2D;
    image_create_info.format        = VK_FORMAT_R8G8B8A8_UNORM;
    image_create_info.extent.width  = in_width;
    image_create_info.extent.height = in_height;
    image_create_info.extent.depth  = 1u;
    image_create_info.mipLevels     = 1u;
    image_create_info.arrayLayers   = 1u;
    image_create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage         = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    return GRenderSystem->GetDeviceAllocator().CreateImage(image_create_info, allocation_create_info);
}

std::optional<VulkanBuffer> Texture::CreateBuffer(VkDeviceSize const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    return GRenderSystem->GetDeviceAllocator().CreateBuffer(buffer_create_info, allocation_create_info);
}

DAEvoid Texture::UploadData(DAEvoid const* in_data, DAEuint64 const in_size) const
{
    auto& device = GRenderSystem->GetDevice();

    auto  buffer         = CreateBuffer(in_size);
    auto* queue          = device.GetTransferQueue();
    auto  command_buffer = device.AllocateCommandBuffer(queue->GetFamilyIndex(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    if (!buffer || !queue || !command_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    memcpy(buffer->GetMappedData(), in_data, in_size);

    CopyBufferToImage(*command_buffer, *buffer);

    VkSubmitInfo submit_info = {};

    submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1u;
    submit_info.pCommandBuffers    = &command_buffer->GetHandle();

    VulkanFence const fence;

    queue->Submit(submit_info, fence.GetHandle());

    fence.Wait();
}

DAEvoid Texture::CopyBufferToImage(VulkanCommandBuffer const& in_command_buffer, VulkanBuffer const& in_buffer) const noexcept
{
    in_command_buffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkImageMemoryBarrier memory_barrier = {};

    memory_barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memory_barrier.srcAccessMask       = 0u;
    memory_barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    memory_barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memory_barrier.image               = m_image->GetHandle();

    memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    memory_barrier.subresourceRange.levelCount = 1u;
    memory_barrier.subresourceRange.layerCount = 1u;

    in_command_buffer.InsertMemoryBarrier(memory_barrier);

    VkBufferImageCopy region = {};

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1u;

    region.imageExtent = m_image->GetExtent();

    in_command_buffer.CopyBufferToImage(in_buffer, *m_image, region);

    memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memory_barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    in_command_buffer.InsertMemoryBarrier(memory_barrier);

    in_command_buffer.End();
}

#pragma warning (disable : 4100)

DAEvoid Texture::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<TextureLoadingDescriptor const&>(in_descriptor);

    auto width  = 0;
    auto height = 0;
    auto comp   = 0;

    auto* pixels = stbi_load(m_loading_descriptor.path.c_str(), &width, &height, &comp, STBI_rgb_alpha);

    m_image = std::make_unique<VulkanImage>(std::move(CreateImage(width, height).value()));

    if (!m_image)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    UploadData(pixels, width * height * comp);
}

DAEvoid Texture::Reload(ResourceManager& in_manager)
{
    auto width  = 0;
    auto height = 0;
    auto comp   = 0;

    auto* pixels = stbi_load(m_loading_descriptor.path.c_str(), &width, &height, &comp, STBI_rgb_alpha);

    UploadData(pixels, width * height * comp);
}

DAEvoid Texture::Unload(ResourceManager& in_manager) noexcept
{
    m_image.reset();
}

#pragma warning (default : 4100)

VulkanImage const& Texture::GetImage() const noexcept
{
    return *m_image;
}

#pragma endregion