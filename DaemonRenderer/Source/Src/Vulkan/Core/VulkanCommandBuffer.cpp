/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Vulkan/Core/VulkanCommandBuffer.hpp"
#include "Vulkan/Core/VulkanImage.hpp"
#include "Vulkan/Core/VulkanBuffer.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"
#include "Vulkan/Utilities/VulkanUtilities.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors

VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer in_handle, VkCommandPool in_command_pool) noexcept:
    m_handle       {in_handle},
    m_command_pool {in_command_pool}
{

}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& in_move) noexcept:
    m_handle       {in_move.m_handle},
    m_command_pool {in_move.m_command_pool}
{
    in_move.m_handle       = nullptr;
    in_move.m_command_pool = nullptr;
}

VulkanCommandBuffer::~VulkanCommandBuffer() noexcept
{
    if (!m_handle || !m_command_pool)
        return;

    vkFreeCommandBuffers(VulkanLoader::GetLoadedDevice(), m_command_pool, 1u, &m_handle);
}

#pragma endregion

#pragma region Methods

DAEbool VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags      const  in_usage_flags,
                                   VkCommandBufferInheritanceInfo const* in_inheritance_info) const noexcept
{
    VkCommandBufferBeginInfo command_buffer_begin_info = {};

    command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags            = in_usage_flags;
    command_buffer_begin_info.pInheritanceInfo = in_inheritance_info;

    if (VK_CHECK(vkBeginCommandBuffer(m_handle, &command_buffer_begin_info)))
        return false;

    return true;
}

DAEbool VulkanCommandBuffer::End() const noexcept
{
    if (VK_CHECK(vkEndCommandBuffer(m_handle)))
        return false;

    return true;
}

DAEvoid VulkanCommandBuffer::BeginLabel(std::string_view const in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name.data();
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkCmdBeginDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanCommandBuffer::InsertLabel(std::string_view const in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name.data();
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkCmdInsertDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanCommandBuffer::EndLabel() const noexcept
{
    vkCmdEndDebugUtilsLabelEXT(m_handle);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkPipelineStageFlags const  in_src_stage,
                                                 VkPipelineStageFlags const  in_dst_stage,
                                                 VkDependencyFlags    const  in_dependency_flags,
                                                 VkMemoryBarrier      const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         in_src_stage,
                         in_dst_stage,
                         in_dependency_flags,
                         1u, &in_memory_barrier,
                         0u, nullptr,
                         0u, nullptr);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkPipelineStageFlags  const  in_src_stage,
                                                 VkPipelineStageFlags  const  in_dst_stage,
                                                 VkDependencyFlags     const  in_dependency_flags,
                                                 VkBufferMemoryBarrier const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         in_src_stage,
                         in_dst_stage,
                         in_dependency_flags,
                         0u, nullptr,
                         1u, &in_memory_barrier,
                         0u, nullptr);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkPipelineStageFlags const  in_src_stage,
                                                 VkPipelineStageFlags const  in_dst_stage,
                                                 VkDependencyFlags    const  in_dependency_flags,
                                                 VkImageMemoryBarrier const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         in_src_stage,
                         in_dst_stage,
                         in_dependency_flags,
                         0u, nullptr,
                         0u, nullptr,
                         1u, &in_memory_barrier);
}

DAEvoid VulkanCommandBuffer::CopyBufferToBuffer(VulkanBuffer const& in_src_buffer,
                                                VulkanBuffer const& in_dst_buffer) const noexcept
{
    if (in_src_buffer.GetSize() != in_dst_buffer.GetSize())
        return;

    VkBufferCopy region = {};

    region.size = in_src_buffer.GetSize();

    vkCmdCopyBuffer(m_handle, in_src_buffer.GetHandle(), in_dst_buffer.GetHandle(), 1u, &region);
}

DAEvoid VulkanCommandBuffer::CopyBufferToBuffer(VulkanBuffer const& in_src_buffer,
                                                VulkanBuffer const& in_dst_buffer,
                                                VkBufferCopy const& in_region) const noexcept
{
    vkCmdCopyBuffer(m_handle, in_src_buffer.GetHandle(), in_dst_buffer.GetHandle(), 1u, &in_region);
}

DAEvoid VulkanCommandBuffer::CopyImageToImage(VulkanImage const& in_src_image,
                                              VulkanImage const& in_dst_image) const noexcept
{
    if (in_src_image.GetExtent() != in_dst_image.GetExtent())
        return;

    VkImageCopy region = {};

    region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.srcSubresource.layerCount = 1u;
    region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.dstSubresource.layerCount = 1u;
    region.extent                    = in_dst_image.GetExtent();

    vkCmdCopyImage(m_handle,
                   in_src_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   in_dst_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1u,
                   &region);
}

DAEvoid VulkanCommandBuffer::CopyImageToImage(VulkanImage const& in_src_image,
                                              VulkanImage const& in_dst_image,
                                              VkImageCopy const& in_region) const noexcept
{
    vkCmdCopyImage(m_handle,
                   in_src_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   in_dst_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1u,
                   &in_region);
}

DAEvoid VulkanCommandBuffer::CopyBufferToImage(VulkanBuffer const& in_buffer,
                                               VulkanImage  const& in_image) const noexcept
{
    VkBufferImageCopy region = {};

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1u;
    region.imageExtent                 = in_image.GetExtent();

    vkCmdCopyBufferToImage(m_handle,
                           in_buffer.GetHandle(),
                           in_image .GetHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1u,
                           &region);
}

DAEvoid VulkanCommandBuffer::CopyBufferToImage(VulkanBuffer      const& in_buffer,
                                               VulkanImage       const& in_image,
                                               VkBufferImageCopy const& in_region) const noexcept
{
    vkCmdCopyBufferToImage(m_handle,
                           in_buffer.GetHandle(),
                           in_image .GetHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1u,
                           &in_region);
}

DAEvoid VulkanCommandBuffer::CopyImageToBuffer(VulkanImage  const& in_image,
                                               VulkanBuffer const& in_buffer) const noexcept
{
    VkBufferImageCopy region = {};

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1u;
    region.imageExtent                 = in_image.GetExtent();

    vkCmdCopyImageToBuffer(m_handle,
                           in_image.GetHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           in_buffer.GetHandle(),
                           1u,
                           &region);
}

DAEvoid VulkanCommandBuffer::CopyImageToBuffer(VulkanImage       const& in_image,
                                               VulkanBuffer      const& in_buffer,
                                               VkBufferImageCopy const& in_region) const noexcept
{
    vkCmdCopyImageToBuffer(m_handle,
                           in_image.GetHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           in_buffer.GetHandle(),
                           1u,
                           &in_region);
}

DAEvoid VulkanCommandBuffer::BlitImage(VulkanImage const& in_src_image,
                                       VulkanImage const& in_dst_image,
                                       VkFilter    const  in_filter) const noexcept
{
    VkImageBlit region = {};

    region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.srcSubresource.layerCount = 1u;
    region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.dstSubresource.layerCount = 1u;

    region.srcOffsets[1] = reinterpret_cast<VkOffset3D const&>(in_src_image.GetExtent());
    region.dstOffsets[1] = reinterpret_cast<VkOffset3D const&>(in_dst_image.GetExtent());

    vkCmdBlitImage(m_handle,
                   in_src_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   in_dst_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1u,
                   &region,
                   in_filter);
}

DAEvoid VulkanCommandBuffer::BlitImage(VulkanImage const& in_src_image,
                                       VulkanImage const& in_dst_image,
                                       VkImageBlit const& in_region,
                                       VkFilter    const  in_filter) const noexcept
{
    vkCmdBlitImage(m_handle,
                   in_src_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   in_dst_image.GetHandle(),
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1u,
                   &in_region,
                   in_filter);
}

VkCommandBuffer const& VulkanCommandBuffer::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion

#pragma region Operators

VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& in_move) noexcept
{
    m_handle       = in_move.m_handle;
    m_command_pool = in_move.m_command_pool;

    in_move.m_handle       = nullptr;
    in_move.m_command_pool = nullptr;

    return *this;
}

#pragma endregion