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

#include "Vulkan/Core/VulkanCommandPool.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

VulkanCommandPool::VulkanCommandPool(DAEuint32 const in_queue_family_index, VkCommandPoolCreateFlags const in_flags)
{
    VkCommandPoolCreateInfo command_pool_create_info = {};

    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags            = in_flags;
    command_pool_create_info.queueFamilyIndex = in_queue_family_index;

    VK_CHECK(vkCreateCommandPool(VulkanLoader::GetLoadedDevice(), &command_pool_create_info, nullptr, &m_handle));
}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& in_move) noexcept:
    m_handle                    {in_move.m_handle},
    m_primary_index             {in_move.m_primary_index},
    m_secondary_index           {in_move.m_secondary_index},
    m_primary_command_buffers   {std::move(in_move.m_primary_command_buffers)},
    m_secondary_command_buffers {std::move(in_move.m_secondary_command_buffers)}
{
    in_move.m_handle = nullptr;
}

VulkanCommandPool::~VulkanCommandPool() noexcept
{
    if (!m_handle)
        return;

    vkDestroyCommandPool(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

std::optional<VulkanCommandBuffer> VulkanCommandPool::AllocateCommandBuffer(VkCommandBufferLevel const in_level) const noexcept
{
    VkCommandBuffer             command_buffer               = nullptr;
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool        = m_handle;
    command_buffer_allocate_info.level              = in_level;
    command_buffer_allocate_info.commandBufferCount = 1u;

    if (vkAllocateCommandBuffers(VulkanLoader::GetLoadedDevice(), &command_buffer_allocate_info, &command_buffer) != VK_SUCCESS)
        return std::nullopt;

    return VulkanCommandBuffer(command_buffer, m_handle);
}

VulkanCommandBuffer* VulkanCommandPool::RequestCommandBuffer(VkCommandBufferLevel const in_level)
{
    if (in_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY && m_primary_index < m_primary_command_buffers.size())
        return &m_primary_command_buffers[m_primary_index++];
    if (in_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY && m_secondary_index < m_secondary_command_buffers.size())
        return &m_secondary_command_buffers[m_primary_index++];

    VkCommandBuffer             command_buffer               = nullptr;
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool        = m_handle;
    command_buffer_allocate_info.level              = in_level;
    command_buffer_allocate_info.commandBufferCount = 1u;

    if (vkAllocateCommandBuffers(VulkanLoader::GetLoadedDevice(), &command_buffer_allocate_info, &command_buffer) != VK_SUCCESS)
        return nullptr;

    if (in_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        m_primary_index++;

        return &m_primary_command_buffers.emplace_back(command_buffer, nullptr);
    }

    if (in_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY)
    {
        m_secondary_index++;

        return &m_secondary_command_buffers.emplace_back(command_buffer, nullptr);
    }

    return nullptr;
}

DAEvoid VulkanCommandPool::Reset(VkCommandPoolResetFlags const in_flags) noexcept
{
    m_primary_index   = 0u;
    m_secondary_index = 0u;

    VK_CHECK(vkResetCommandPool(VulkanLoader::GetLoadedDevice(), m_handle, in_flags));
}

#pragma endregion