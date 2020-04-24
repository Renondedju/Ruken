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

#include "Vulkan/Core/VulkanQueue.hpp"
#include "Vulkan/Core/VulkanCommandBuffer.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

VulkanQueue::VulkanQueue(VkPhysicalDevice   in_physical_device,
                         VkQueue            in_handle,
                         DAEuint32    const in_family_index,
                         DAEuint32    const in_index,
                         VkQueueFlags const in_flags) noexcept:
    m_physical_device   {in_physical_device},
    m_handle            {in_handle},
    m_family_index      {in_family_index},
    m_index             {in_index},
    m_flags             {in_flags}
{

}

#pragma endregion

#pragma region Methods

DAEvoid VulkanQueue::BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name;
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkQueueBeginDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanQueue::EndLabel() const noexcept
{
    vkQueueEndDebugUtilsLabelEXT(m_handle);
}

DAEvoid VulkanQueue::InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
{
    VkDebugUtilsLabelEXT label_info = {};

    label_info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label_info.pLabelName = in_label_name;
    label_info.color[0]   = in_color.data[0];
    label_info.color[1]   = in_color.data[1];
    label_info.color[2]   = in_color.data[2];
    label_info.color[3]   = in_color.data[3];

    vkQueueInsertDebugUtilsLabelEXT(m_handle, &label_info);
}

DAEvoid VulkanQueue::Present(VkPresentInfoKHR const& in_present_info) const noexcept
{
    VK_CHECK(vkQueuePresentKHR(m_handle, &in_present_info));
}

DAEvoid VulkanQueue::Submit(VkSubmitInfo const& in_submit_info, VkFence in_fence) const noexcept
{
    VK_CHECK(vkQueueSubmit(m_handle, 1u, &in_submit_info, in_fence));
}

DAEvoid VulkanQueue::Submit(std::vector<VkSubmitInfo> const& in_submit_infos, VkFence in_fence) const noexcept
{
    VK_CHECK(vkQueueSubmit(m_handle, static_cast<DAEuint32>(in_submit_infos.size()), in_submit_infos.data(), in_fence));
}

DAEvoid VulkanQueue::Submit(VulkanCommandBuffer const& in_command_buffer, VkFence in_fence) const noexcept
{
    VkSubmitInfo submit_info = {};

    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount   = 1u;
    submit_info.pCommandBuffers      = &in_command_buffer.GetHandle();

    VK_CHECK(vkQueueSubmit(m_handle, 1u, &submit_info, in_fence));
}

DAEvoid VulkanQueue::WaitIdle() const noexcept
{
     VK_CHECK(vkQueueWaitIdle(m_handle));
}

VkQueue const& VulkanQueue::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 VulkanQueue::GetFamilyIndex() const noexcept
{
    return m_family_index;
}

DAEuint32 VulkanQueue::GetIndex() const noexcept
{
    return m_index;
}

VkQueueFlags VulkanQueue::GetFlags() const noexcept
{
    return m_flags;
}

DAEbool VulkanQueue::IsPresentationSupported(VkSurfaceKHR in_surface) const noexcept
{
    VkBool32 presentation_support = VK_FALSE;

    VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, m_family_index, in_surface, &presentation_support));

    return presentation_support;
}

#pragma endregion