
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

RkBool SemaphorePool::Reset() noexcept
{
    auto const timeline_semaphore_count = m_timeline_semaphores.size();

    {
        std::lock_guard lock(m_mutex);

        m_timeline_semaphores.clear();

        for (RkSize i = 0; i < timeline_semaphore_count; ++i)
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