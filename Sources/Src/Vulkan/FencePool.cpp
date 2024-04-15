
#include "Vulkan/FencePool.hpp"


#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

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

RkBool FencePool::Wait() const noexcept
{
    std::lock_guard lock(m_mutex);

    std::vector<VkFence> handles(m_fences.size());

    for (auto const& fence : m_fences)
        handles.emplace_back(fence.GetHandle());

    if (VK_CHECK(vkWaitForFences(VulkanLoader::GetLoadedDevice(), static_cast<RkUint32>(handles.size()), handles.data(), VK_TRUE, UINT64_MAX)))
        return false;

    return true;
}

RkBool FencePool::Reset() noexcept
{
    {
        std::lock_guard lock(m_mutex);

        std::vector<VkFence> handles(m_fences.size());

        for (auto const& fence : m_fences)
            handles.emplace_back(fence.GetHandle());

        if (VK_CHECK(vkResetFences(VulkanLoader::GetLoadedDevice(), static_cast<RkUint32>(handles.size()), handles.data())))
            return false;
    }

    m_index.store(0u, std::memory_order_release);

    return true;
}

#pragma endregion