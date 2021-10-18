#include "Rendering/RenderObjectPools/SemaphorePool.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

SemaphorePool::SemaphorePool(RenderDevice* in_device) noexcept: RenderObjectPool<vk::Semaphore>(in_device)
{
    Grow();
}

SemaphorePool::~SemaphorePool() noexcept
{
    for (auto const& semaphore : m_objects)
    {
        if (semaphore)
            m_device->GetLogicalDevice().destroy(semaphore);
    }
}

RkVoid SemaphorePool::Grow()
{
    vk::SemaphoreTypeCreateInfo semaphore_type_create_info = {
        .semaphoreType = vk::SemaphoreType::eBinary,
    };

    vk::SemaphoreCreateInfo semaphore_create_info = {
        .pNext = &semaphore_type_create_info
    };

    for (RkUint32 i = 0U; i < 4U; ++i)
    {
        auto [result, value] = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info);

        if (result != vk::Result::eSuccess)
            continue;

        m_objects.emplace_back(value);
    }
}

RkVoid SemaphorePool::Reset()
{
    
}