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

#include "Vulkan/FencePool.hpp"


#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

VulkanFence& FencePool::RequestFence() noexcept
{
    if (m_index.load(std::memory_order_acquire) >= m_fences.size())
    {
        std::lock_guard lock(m_mutex);

        m_fences.emplace_back();
    }

    return m_fences[m_index.fetch_add(1u, std::memory_order_release)];
}

DAEbool FencePool::Wait() const noexcept
{
    std::lock_guard lock(m_mutex);

    std::vector<VkFence> handles(m_fences.size());

    for (auto const& fence : m_fences)
        handles.emplace_back(fence.GetHandle());

    if (VK_CHECK(vkWaitForFences(VulkanLoader::GetLoadedDevice(), static_cast<DAEuint32>(handles.size()), handles.data(), VK_TRUE, UINT64_MAX)))
        return false;

    return true;
}

DAEbool FencePool::Reset() noexcept
{
    {
        std::lock_guard lock(m_mutex);

        std::vector<VkFence> handles(m_fences.size());

        for (auto const& fence : m_fences)
            handles.emplace_back(fence.GetHandle());

        if (VK_CHECK(vkResetFences(VulkanLoader::GetLoadedDevice(), static_cast<DAEuint32>(handles.size()), handles.data())))
            return false;
    }

    m_index.store(0u, std::memory_order_release);

    return true;
}

#pragma endregion