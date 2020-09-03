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

#include "Vulkan/SemaphorePool.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

VulkanSemaphore& SemaphorePool::RequestSemaphore() noexcept
{
    if (m_semaphore_index.load(std::memory_order_acquire) >= m_semaphores.size())
    {
        std::lock_guard lock(m_mutex);

        m_semaphores.emplace_back();
    }

    return m_semaphores[m_semaphore_index.fetch_add(1u, std::memory_order_release)];
}

VulkanTimelineSemaphore& SemaphorePool::RequestTimelineSemaphore() noexcept
{
    if (m_timeline_semaphore_index.load(std::memory_order_acquire) >= m_timeline_semaphores.size())
    {
        std::lock_guard lock(m_mutex);

        m_timeline_semaphores.emplace_back();
    }

    return m_timeline_semaphores[m_timeline_semaphore_index.fetch_add(1u, std::memory_order_release)];
}

DAEbool SemaphorePool::Reset() noexcept
{
    auto const timeline_semaphore_count = m_timeline_semaphores.size();

    {
        std::lock_guard lock(m_mutex);

        m_timeline_semaphores.clear();

        for (DAEsize i = 0; i < timeline_semaphore_count; ++i)
        {
            if (!m_timeline_semaphores.emplace_back().GetHandle())
                return false;
        }
    }

    m_semaphore_index         .store(0u, std::memory_order_release);
    m_timeline_semaphore_index.store(0u, std::memory_order_release);

    return true;
}

#pragma endregion