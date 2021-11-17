#include "Rendering/RenderQueue.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

RenderQueue::RenderQueue(Logger* in_logger, RenderDevice* in_device, RkUint32 const in_family_index, RkUint32 const in_queue_index) noexcept:
    m_logger       {in_logger},
    m_device       {in_device},
    m_family_index {in_family_index},
    m_queue        {in_device->GetLogicalDevice().getQueue(in_family_index, in_queue_index)}
{

}

RenderQueue::~RenderQueue() noexcept
{
    for (auto const& [id, fence] : m_fences)
    {
        m_device->GetLogicalDevice().destroy(fence);
    }

    for (auto const& [id, command_pool] : m_command_pools)
    {
        m_device->GetLogicalDevice().destroy(command_pool);
    }
}

#pragma endregion

#pragma region Methods

RkVoid RenderQueue::Submit(vk::SubmitInfo2KHR const& in_submit_info) noexcept
{
    std::lock_guard lock(m_mutex);

    if (m_queue.submit2KHR(in_submit_info) != vk::Result::eSuccess)
    {

    }
}

RkVoid RenderQueue::Present(vk::PresentInfoKHR const& in_present_info) noexcept
{
    std::lock_guard lock(m_mutex);

    if (m_queue.presentKHR(in_present_info) != vk::Result::eSuccess)
    {
        
    }
}

vk::CommandBuffer RenderQueue::BeginSingleUseCommandBuffer() noexcept
{
    if (!m_command_pools.contains(std::this_thread::get_id()))
    {
        vk::FenceCreateInfo fence_create_info = {};

        if (auto [result, value] = m_device->GetLogicalDevice().createFence(fence_create_info); result == vk::Result::eSuccess)
        {
            m_fences[std::this_thread::get_id()] = value;
        }

        else
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create fence : " + vk::to_string(result)))

        vk::CommandPoolCreateInfo command_pool_create_info = {
            .flags            = vk::CommandPoolCreateFlagBits::eTransient,
            .queueFamilyIndex = m_family_index
        };

        if (auto [result, value] = m_device->GetLogicalDevice().createCommandPool(command_pool_create_info); result == vk::Result::eSuccess)
        {
            m_command_pools[std::this_thread::get_id()] = value;
        }

        else
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create command pool : " + vk::to_string(result)))
    }

    vk::CommandBufferAllocateInfo command_buffer_allocate_info = {
        .commandPool        = m_command_pools[std::this_thread::get_id()],
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1U
    };

    if (auto [result, value] = m_device->GetLogicalDevice().allocateCommandBuffers(command_buffer_allocate_info); result == vk::Result::eSuccess)
    {
        auto const command_buffer = value.front();

        vk::CommandBufferBeginInfo command_buffer_begin_info = {
            .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };
        
        if (command_buffer.begin(command_buffer_begin_info) == vk::Result::eSuccess)
            return command_buffer;

        m_device->GetLogicalDevice().freeCommandBuffers(m_command_pools.at(std::this_thread::get_id()), command_buffer);
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to allocate command buffer : " + vk::to_string(result)))

    return VK_NULL_HANDLE;
}

RkVoid RenderQueue::EndSingleUseCommandBuffer(vk::CommandBuffer const& in_command_buffer) const noexcept
{
    if (in_command_buffer.end() == vk::Result::eSuccess)
    {
        auto const& fence = m_fences.at(std::this_thread::get_id());

        vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
            .commandBuffer = in_command_buffer
        };

        vk::SubmitInfo2KHR submit_info = {
            .commandBufferInfoCount = 1U,
            .pCommandBufferInfos    = &command_buffer_submit_info
        };

        if (m_queue.submit2KHR(submit_info, fence) == vk::Result::eSuccess)
        {
            if (m_device->GetLogicalDevice().waitForFences(fence, VK_TRUE, UINT64_MAX) == vk::Result::eSuccess)
                m_device->GetLogicalDevice().resetFences  (fence);
        }
    }

    m_device->GetLogicalDevice().freeCommandBuffers(m_command_pools.at(std::this_thread::get_id()), in_command_buffer);
}

#pragma endregion