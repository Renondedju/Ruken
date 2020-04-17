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

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

CommandPool::CommandPool(DAEuint32 const in_queue_family_index):
    m_queue_family_index {in_queue_family_index}
{
    VkCommandPoolCreateInfo command_pool_create_info = {};

    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags            = 0u;
    command_pool_create_info.queueFamilyIndex = in_queue_family_index;

    // TODO : add for-loop to create a command pool for each available thread.
    {
        CommandPoolData data;

        if (vkCreateCommandPool(VulkanLoader::GetLoadedDevice(), &command_pool_create_info, nullptr, &data.handle) != VK_SUCCESS)
            throw std::exception("Failed to create a command pool!");

        m_handles.emplace(std::this_thread::get_id(), std::move(data));
    }
}

CommandPool::~CommandPool() noexcept
{
    for (auto const& handle : m_handles)
        vkDestroyCommandPool(VulkanLoader::GetLoadedDevice(), handle.second.handle, nullptr);
}

#pragma endregion

#pragma region Methods

CommandBuffer& CommandPool::RequestCommandBuffer()
{
    auto const it = m_handles.find(std::this_thread::get_id());

    if (it == m_handles.cend())
        throw std::exception("Requesting command buffer from an unregistered thread!");

    auto const& command_pool = it->second;

    if (command_pool.index == m_handles.size())
    {
        VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

        command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.commandPool        = command_pool.handle;
        command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = 1u;

        VkCommandBuffer handle;

        if (vkAllocateCommandBuffers(VulkanLoader::GetLoadedDevice(), &command_buffer_allocate_info, &handle) != VK_SUCCESS)
            throw std::exception("Failed to allocate a command buffer!");

        it->second.command_buffers.emplace_back(handle);
    }

    return it->second.command_buffers[it->second.index++];
}

DAEvoid CommandPool::Reset()
{
    for (auto& handle : m_handles)
    {
        handle.second.index = 0u;

        if (vkResetCommandPool(VulkanLoader::GetLoadedDevice(), handle.second.handle, 0u) != VK_SUCCESS)
            throw std::exception("Failed to reset a command pool!");
    }
}

DAEuint32 CommandPool::GetQueueFamilyIndex() const noexcept
{
    return m_queue_family_index;
}

#pragma endregion