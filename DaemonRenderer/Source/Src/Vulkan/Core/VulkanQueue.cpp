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
#include "Vulkan/Core/VulkanSemaphore.hpp"
#include "Vulkan/Core/VulkanSwapchain.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

VulkanQueue::VulkanQueue(VkPhysicalDevice   in_physical_device,
                         VkQueue            in_handle,
                         DAEuint32 const    in_queue_family) noexcept:
    m_physical_device   {in_physical_device},
    m_handle            {in_handle},
    m_queue_family      {in_queue_family}
{

}

VulkanQueue::VulkanQueue(VulkanQueue&& in_move) noexcept:
    m_physical_device   {in_move.m_physical_device},
    m_handle            {in_move.m_handle},
    m_queue_family      {in_move.m_queue_family}
{
    in_move.m_physical_device = nullptr;
    in_move.m_handle          = nullptr;
    in_move.m_queue_family    = UINT32_MAX;
}

VulkanQueue::~VulkanQueue() noexcept
{
    if (!m_handle)
        return;

    WaitIdle();
}

#pragma endregion

#pragma region Methods

DAEvoid VulkanQueue::WaitIdle() const noexcept
{
    std::lock_guard lock(m_mutex);

    VK_CHECK(vkQueueWaitIdle(m_handle));
}

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

DAEvoid VulkanQueue::EndLabel() const noexcept
{
    vkQueueEndDebugUtilsLabelEXT(m_handle);
}


DAEbool VulkanQueue::Submit(VulkanCommandBuffer const& in_command_buffer, VkFence in_fence) const noexcept
{
    VkSubmitInfo submit_info = {};

    submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1u;
    submit_info.pCommandBuffers    = &in_command_buffer.GetHandle();

    std::lock_guard lock(m_mutex);

    if (VK_CHECK(vkQueueSubmit(m_handle, 1u, &submit_info, in_fence)))
        return false;

    return true;
}

DAEbool VulkanQueue::Submit(VkSubmitInfo const& in_submit_info, VkFence in_fence) const noexcept
{
    std::lock_guard lock(m_mutex);

    if (VK_CHECK(vkQueueSubmit(m_handle, 1u, &in_submit_info, in_fence)))
        return false;

    return true;
}

DAEbool VulkanQueue::Submit(std::vector<VkSubmitInfo> const& in_submit_infos, VkFence in_fence) const noexcept
{
    std::lock_guard lock(m_mutex);

    if (VK_CHECK(vkQueueSubmit(m_handle, static_cast<DAEuint32>(in_submit_infos.size()), in_submit_infos.data(), in_fence)))
        return false;

    return true;
}

DAEbool VulkanQueue::Present(VulkanSwapchain const& in_swapchain, VulkanSemaphore const& in_semaphore) const noexcept
{
    VkPresentInfoKHR present_info = {};

    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1u;
    present_info.pWaitSemaphores    = &in_semaphore.GetHandle();
    present_info.swapchainCount     = 1u;
    present_info.pSwapchains        = &in_swapchain.GetHandle();
    present_info.pImageIndices      = &in_swapchain.GetImageIndex();

    std::lock_guard lock(m_mutex);

    if (VK_CHECK(vkQueuePresentKHR(m_handle, &present_info)))
        return false;

    return true;
}

DAEbool VulkanQueue::Present(VkPresentInfoKHR const& in_present_info) const noexcept
{
    std::lock_guard lock(m_mutex);

    if (VK_CHECK(vkQueuePresentKHR(m_handle, &in_present_info)))
        return false;

    return true;
}

DAEbool VulkanQueue::IsPresentationSupported(VkSurfaceKHR in_surface) const noexcept
{
    VkBool32 presentation_support = VK_FALSE;

    if (VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, m_queue_family, in_surface, &presentation_support)))
        return false;

    return presentation_support;
}

VkQueue const& VulkanQueue::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 VulkanQueue::GetQueueFamily() const noexcept
{
    return m_queue_family;
}

#pragma endregion

#pragma region Operators

VulkanQueue& VulkanQueue::operator=(VulkanQueue&& in_move) noexcept
{
    m_handle          = in_move.m_handle;
    m_physical_device = in_move.m_physical_device;
    m_queue_family    = in_move.m_queue_family;

    in_move.m_handle          = nullptr;
    in_move.m_physical_device = nullptr;
    in_move.m_queue_family    = UINT32_MAX;

    return *this;
}

#pragma endregion