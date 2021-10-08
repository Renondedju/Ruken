#include "Rendering/RenderObjects/FencePool.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

FencePool::FencePool(RenderDevice* in_device) noexcept: RenderObjectPool<vk::Fence>(in_device)
{
    Grow();
}

FencePool::~FencePool() noexcept
{
    for (auto const& fence : m_objects)
    {
        if (fence)
            m_device->GetLogicalDevice().destroy(fence);
    }
}

RkVoid FencePool::Grow()
{
    vk::FenceCreateInfo fence_create_info = {
        .flags = vk::FenceCreateFlagBits::eSignaled
    };

    for (RkUint32 i = 0U; i < 4U; ++i)
    {
        auto [result, value] = m_device->GetLogicalDevice().createFence(fence_create_info);

        if (result != vk::Result::eSuccess)
            continue;

        m_objects.emplace_back(value);
    }
}

RkVoid FencePool::Reset()
{
    std::vector fences(m_objects.begin(), m_objects.end());

    m_device->GetLogicalDevice().resetFences(fences);
}