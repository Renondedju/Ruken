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

#include "Vulkan/CommandBuffer.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

CommandBuffer::CommandBuffer(VkCommandBuffer in_handle) noexcept : m_handle { in_handle }
{

}

CommandBuffer::~CommandBuffer()
{

}

#pragma endregion

#pragma region Methods

DAEvoid CommandBuffer::BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
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

DAEvoid CommandBuffer::EndLabel() const noexcept
{
    vkCmdEndDebugUtilsLabelEXT(m_handle);
}

DAEvoid CommandBuffer::InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
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

DAEvoid CommandBuffer::Begin(VkCommandBufferUsageFlags const in_usage_flags) const noexcept
{
    VkCommandBufferBeginInfo command_buffer_begin_info = {};

    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = in_usage_flags;

    vkBeginCommandBuffer(m_handle, &command_buffer_begin_info);
}

DAEvoid CommandBuffer::End() const noexcept
{
    vkEndCommandBuffer(m_handle);
}

DAEvoid CommandBuffer::Reset(VkCommandBufferResetFlags const in_reset_flags) const noexcept
{
    vkResetCommandBuffer(m_handle, in_reset_flags);
}

VkCommandBuffer const& CommandBuffer::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion