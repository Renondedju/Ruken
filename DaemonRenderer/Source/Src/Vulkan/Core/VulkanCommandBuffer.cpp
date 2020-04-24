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

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer in_handle, VkCommandPool in_command_pool) noexcept:
    m_handle        {in_handle},
    m_command_pool  {in_command_pool}
{

}

VulkanCommandBuffer::~VulkanCommandBuffer() noexcept
{
    if (!m_handle || !m_command_pool)
        return;

    vkFreeCommandBuffers(VulkanLoader::GetLoadedDevice(), m_command_pool, 1u, &m_handle);
}

#pragma endregion

#pragma region Methods

DAEvoid VulkanCommandBuffer::BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name;
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkCmdBeginDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanCommandBuffer::EndLabel() const noexcept
{
    vkCmdEndDebugUtilsLabelEXT(m_handle);
}

DAEvoid VulkanCommandBuffer::InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name;
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkCmdInsertDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags const in_usage_flags) const noexcept
{
    VkCommandBufferBeginInfo command_buffer_begin_info = {};

    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = in_usage_flags;

    vkBeginCommandBuffer(m_handle, &command_buffer_begin_info);
}

DAEvoid VulkanCommandBuffer::End() const noexcept
{
    vkEndCommandBuffer(m_handle);
}

DAEvoid VulkanCommandBuffer::Reset(VkCommandBufferResetFlags const in_reset_flags) const noexcept
{
    vkResetCommandBuffer(m_handle, in_reset_flags);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkMemoryBarrier const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0u,
                         1u, &in_memory_barrier,
                         0u, nullptr,
                         0u, nullptr);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkBufferMemoryBarrier const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0u,
                         0u, nullptr,
                         1u, &in_memory_barrier,
                         0u, nullptr);
}

DAEvoid VulkanCommandBuffer::InsertMemoryBarrier(VkImageMemoryBarrier const& in_memory_barrier) const noexcept
{
    vkCmdPipelineBarrier(m_handle,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0u,
                         0u, nullptr,
                         0u, nullptr,
                         1u, &in_memory_barrier);
}

DAEvoid VulkanCommandBuffer::CopyBuffer(VulkanBuffer const& in_src_buffer,
                                        VulkanBuffer const& in_dst_buffer,
                                        VkBufferCopy const& in_region) const noexcept
{
    vkCmdCopyBuffer(m_handle, in_src_buffer.GetHandle(), in_dst_buffer.GetHandle(), 1u, &in_region);
}

DAEvoid VulkanCommandBuffer::CopyImage(VulkanImage const& in_src_image,
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

DAEvoid VulkanCommandBuffer::CopyBufferToImage(VulkanBuffer      const& in_buffer,
                                               VulkanImage       const& in_image,
                                               VkBufferImageCopy const& in_region) const noexcept
{
    vkCmdCopyBufferToImage(m_handle,
                           in_buffer.GetHandle(),
                           in_image.GetHandle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1u,
                           &in_region);
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

VkCommandBuffer const& VulkanCommandBuffer::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion