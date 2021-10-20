#include "Rendering/RenderObjectPools/CommandPool.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

CommandPool::CommandPool(RenderDevice* in_device, RkUint32 in_family_index, vk::CommandPoolCreateFlags in_flags) noexcept:
    RenderObjectPool<vk::CommandBuffer>(in_device)
{
    vk::CommandPoolCreateInfo command_pool_create_info = {
        .flags            = in_flags,
        .queueFamilyIndex = in_family_index
    };

    if (auto [result, value] = m_device->GetLogicalDevice().createCommandPool(command_pool_create_info); result == vk::Result::eSuccess)
    {
        m_handle = value;
    }

    Grow();
}

CommandPool::~CommandPool() noexcept
{
    if (m_handle)
        m_device->GetLogicalDevice().destroy(m_handle);
}

RkVoid CommandPool::Grow()
{
    if (!m_handle)
        return;

    vk::CommandBufferAllocateInfo command_buffer_allocate_info = {
        .commandPool        = m_handle,
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 4U
    };

    if (auto [result, value] = m_device->GetLogicalDevice().allocateCommandBuffers(command_buffer_allocate_info); result == vk::Result::eSuccess)
    {
        for (auto const& command_buffer : value)
        {
            m_objects.emplace_back(command_buffer);
        }
    }
}

RkVoid CommandPool::Reset()
{
    m_device->GetLogicalDevice().resetCommandPool(m_handle);
}