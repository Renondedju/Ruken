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

#include "Vulkan/CommandPool.hpp"

#include "Threading/Scheduler.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors

CommandPool::CommandPool(Scheduler& in_scheduler, DAEuint32 const in_queue_family_index) noexcept
{
    // Creates a command pool for each worker.
    for (auto const& worker : in_scheduler.GetWorkers())
    {
        CommandPoolData data = {};

        data.pool = std::make_unique<VulkanCommandPool>(in_queue_family_index);

        m_command_pools.emplace(worker.ID(), std::move(data));
    }

    // Creates a command pool for the main thread since it is not managed by the Scheduler.
    CommandPoolData data = {};

    data.pool = std::make_unique<VulkanCommandPool>(in_queue_family_index);

    m_command_pools.emplace(std::this_thread::get_id(), std::move(data));
}

#pragma endregion

#pragma region Methods

VulkanCommandBuffer* CommandPool::RequestCommandBuffer(VkCommandBufferLevel const in_level) noexcept
{
    auto& data = m_command_pools.at(std::this_thread::get_id());

    if (in_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        // Returns a command buffer from the pool if one is available.
        if (data.primary_index < data.primary_command_buffers.size())
            return data.primary_command_buffers[data.primary_index++].get();

        // Allocates a new command buffer if none is available.
        if (auto command_buffer = data.pool->AllocateCommandBuffer(in_level))
        {
            data.primary_index++;

            return data.primary_command_buffers.emplace_back(std::make_unique<VulkanCommandBuffer>(std::move(*command_buffer))).get();
        }
    }

    else
    {
        // Returns a command buffer from the pool if one is available.
        if (data.secondary_index < data.second_command_buffers.size())
            return data.second_command_buffers[data.secondary_index++].get();

        // Allocates a new command buffer if none is available.
        if (auto command_buffer = data.pool->AllocateCommandBuffer(in_level))
        {
            data.secondary_index++;

            return data.second_command_buffers.emplace_back(std::make_unique<VulkanCommandBuffer>(std::move(*command_buffer))).get();
        }
    }

    return nullptr;
}

DAEbool CommandPool::Reset() noexcept
{
    for (auto& it : m_command_pools)
    {
        // Resets the indexes of a pool if it could be reset.
        if (VK_CHECK(vkResetCommandPool(VulkanLoader::GetLoadedDevice(), it.second.pool->GetHandle(), 0u)))
            return false;

        it.second.primary_index   = 0u;
        it.second.secondary_index = 0u;
    }

    return true;
}

#pragma endregion