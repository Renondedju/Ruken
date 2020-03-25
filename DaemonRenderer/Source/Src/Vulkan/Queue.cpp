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

#include "Vulkan/Queue.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

Queue::Queue(Device*            in_device,
             VkQueue            in_handle,
             DAEuint32    const in_queue_family_index,
             DAEuint32    const in_index,
             VkQueueFlags const in_flags) noexcept :
    m_device        { in_device },
    m_handle        { in_handle },
    m_family_index  { in_queue_family_index },
    m_index         { in_index },
    m_flags         { in_flags }
{

}
#pragma endregion

#pragma region Methods

DAEvoid Queue::BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
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

DAEvoid Queue::EndLabel() const noexcept
{
    vkQueueEndDebugUtilsLabelEXT(m_handle);
}

DAEvoid Queue::InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept
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

DAEbool Queue::Present(VkPresentInfoKHR const& in_present_info) const noexcept
{
    return vkQueuePresentKHR(m_handle, &in_present_info) == VK_SUCCESS;
}

DAEbool Queue::Submit(DAEuint32 const in_submit_count, VkSubmitInfo const& in_submit_infos, VkFence in_fence) const noexcept
{
    return vkQueueSubmit(m_handle, in_submit_count, &in_submit_infos, in_fence) == VK_SUCCESS;
}

DAEvoid Queue::WaitIdle() const noexcept
{
    vkQueueWaitIdle(m_handle);
}

Device* Queue::GetDevice() const noexcept
{
    return m_device;
}

VkQueue Queue::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 Queue::GetFamilyIndex() const noexcept
{
    return m_family_index;
}

DAEuint32 Queue::GetIndex() const noexcept
{
    return m_index;
}

VkQueueFlags Queue::GetFlags() const noexcept
{
    return m_flags;
}

#pragma endregion