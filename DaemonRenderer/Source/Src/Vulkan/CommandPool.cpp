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

#include "Vulkan/CommandPool.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

CommandPool::CommandPool(Device* in_device, DAEuint32 const in_queue_family_index) noexcept :
    m_device                { in_device },
    m_handle                { nullptr },
    m_queue_family_index    { in_queue_family_index }
{

}

CommandPool::~CommandPool()
{
    if (m_handle)
    {
        vkDestroyCommandPool(m_device->GetHandle(), m_handle, nullptr);
    }
}

#pragma endregion

#pragma region Methods

DAEbool CommandPool::Create(Device* in_device, VkCommandPoolCreateInfo const& in_create_info, CommandPool** out_command_pool)
{
    *out_command_pool = new CommandPool(in_device, in_create_info.queueFamilyIndex);

    return vkCreateCommandPool(in_device->GetHandle(), &in_create_info, nullptr, &(*out_command_pool)->m_handle) == VK_SUCCESS;
}

DAEbool CommandPool::AllocateCommandBuffers(VkCommandBufferAllocateInfo& in_allocate_infos, VkCommandBuffer* out_command_buffers) const
{
    in_allocate_infos.commandPool = m_handle;

    return vkAllocateCommandBuffers(m_device->GetHandle(), &in_allocate_infos, out_command_buffers) == VK_SUCCESS;
}

DAEvoid CommandPool::FreeCommandBuffers(DAEuint32 in_count, VkCommandBuffer* in_command_buffers) const noexcept
{
    vkFreeCommandBuffers(m_device->GetHandle(), m_handle, in_count, in_command_buffers);
}

DAEvoid CommandPool::Trim(VkCommandPoolTrimFlags const in_trim_flags) const noexcept
{
    vkTrimCommandPool(m_device->GetHandle(), m_handle, in_trim_flags);
}

Device* CommandPool::GetDevice() const noexcept
{
    return m_device;
}

VkCommandPool CommandPool::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 CommandPool::GetQueueFamilyIndex() const noexcept
{
    return m_queue_family_index;
}

#pragma endregion