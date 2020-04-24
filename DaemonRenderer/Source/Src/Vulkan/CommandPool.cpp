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

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

CommandPool::CommandPool(DAEuint32 const in_queue_family_index) noexcept
{
    m_command_pools.emplace(std::this_thread::get_id(), VulkanCommandPool(in_queue_family_index));
}

#pragma endregion

#pragma region Methods

VulkanCommandBuffer* CommandPool::RequestCommandBuffer(VkCommandBufferLevel const in_level) noexcept
{
    auto const it = m_command_pools.find(std::this_thread::get_id());

    if (it == m_command_pools.cend())
        return nullptr;

    return it->second.RequestCommandBuffer(in_level);
}

DAEvoid CommandPool::Reset()
{
    for (auto& command_pool : m_command_pools)
        command_pool.second.Reset();
}

#pragma endregion