#include <ranges>

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
    for (auto const& fence : m_fences | std::views::values)
    {
        m_device->GetLogicalDevice().destroy(fence);
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

vk::CommandBuffer RenderQueue::RequestCommandBuffer() noexcept
{
    if (!m_command_pools.contains(std::this_thread::get_id()))
    {
        vk::FenceCreateInfo fence_create_info = {};

        if (auto [result, value] = m_device->GetLogicalDevice().createFence(fence_create_info); result == vk::Result::eSuccess)
        {
            m_fences[std::this_thread::get_id()] = value;
        }

        m_command_pools[std::this_thread::get_id()] = std::make_unique<CommandPool>(m_device, vk::CommandPoolCreateFlagBits::eResetCommandBuffer, m_family_index);
    }

    auto command_buffer = m_command_pools[std::this_thread::get_id()]->Request();

    vk::CommandBufferBeginInfo command_buffer_begin_info = {
        .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    };
    
    command_buffer.begin(command_buffer_begin_info);

    return command_buffer;
}

RkVoid RenderQueue::ReleaseCommandBuffer(vk::CommandBuffer&& in_command_buffer) noexcept
{
    in_command_buffer.end();

    vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
        .commandBuffer = in_command_buffer
    };

    vk::SubmitInfo2KHR submit_info = {
        .commandBufferInfoCount = 1U,
        .pCommandBufferInfos    = &command_buffer_submit_info
    };

    if (m_queue.submit2KHR(submit_info, m_fences[std::this_thread::get_id()]) == vk::Result::eSuccess)
    {
        if (m_device->GetLogicalDevice().waitForFences(m_fences[std::this_thread::get_id()], VK_TRUE, UINT64_MAX) == vk::Result::eSuccess)
        {
            m_device->GetLogicalDevice().resetFences(m_fences[std::this_thread::get_id()]);
        }
    }

    m_command_pools[std::this_thread::get_id()]->Release(std::move(in_command_buffer));
}