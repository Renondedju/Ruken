#include "Rendering/RenderQueue.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

RenderQueue::RenderQueue(Logger* in_logger, RenderDevice* in_device, RkUint32 const in_family_index, RkUint32 const in_queue_index) noexcept:
    m_logger       {in_logger},
    m_device       {in_device},
    m_family_index {in_family_index}
{
    m_queue = m_device->GetLogicalDevice().getQueue(in_family_index, in_queue_index);
}

RenderQueue::~RenderQueue() noexcept
{
    for (auto const& command_pool : m_command_pools)
    {
        if (command_pool.second)
            m_device->GetLogicalDevice().destroy(command_pool.second);
    }
}

RkVoid RenderQueue::Submit(vk::SubmitInfo const& in_submit_info) noexcept
{
    std::lock_guard lock(m_mutex);

    auto result = m_queue.submit(in_submit_info);
}

RkVoid RenderQueue::Submit(vk::SubmitInfo2KHR const& in_submit_info) noexcept
{
    std::lock_guard lock(m_mutex);

    auto result = m_queue.submit2KHR(in_submit_info);
}

RkVoid RenderQueue::Present(vk::PresentInfoKHR const& in_present_info) noexcept
{
    std::lock_guard lock(m_mutex);

    auto result = m_queue.presentKHR(in_present_info);
}

RkVoid RenderQueue::WaitIdle() noexcept
{
    std::lock_guard lock(m_mutex);

    auto result = m_queue.waitIdle();
}

vk::CommandBuffer RenderQueue::AcquireSingleUseCommandBuffer() noexcept
{
    if (!m_command_pools.contains(std::this_thread::get_id()))
    {
        vk::CommandPoolCreateInfo command_pool_create_info = {
            .flags            = vk::CommandPoolCreateFlagBits::eTransient,
            .queueFamilyIndex = m_family_index
        };

        auto [result, value] = m_device->GetLogicalDevice().createCommandPool(command_pool_create_info);

        if (result == vk::Result::eSuccess)
           m_command_pools[std::this_thread::get_id()] = value;
        else
            RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan command pool : " + vk::to_string(result)))
    }

    vk::CommandBufferAllocateInfo command_buffer_allocate_info = {
        .commandPool        = m_command_pools[std::this_thread::get_id()],
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1U
    };

    auto [result, value] = m_device->GetLogicalDevice().allocateCommandBuffers(command_buffer_allocate_info);

    vk::CommandBufferBeginInfo begin_info = {
        .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    };

    value.front().begin(begin_info);

    return value.front();
}

RkVoid RenderQueue::ReleaseSingleUseCommandBuffer(vk::CommandBuffer const& in_command_buffer) noexcept
{
    in_command_buffer.end();

    vk::SubmitInfo submit_info = {
        .commandBufferCount = 1,
        .pCommandBuffers = &in_command_buffer
    };

    m_queue.submit(submit_info);
    m_queue.waitIdle();

    m_device->GetLogicalDevice().freeCommandBuffers(m_command_pools[std::this_thread::get_id()], in_command_buffer);
}