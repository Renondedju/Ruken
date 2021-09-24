#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(RenderDevice* in_device, Logger* in_logger) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    vk::CommandBufferAllocateInfo allocate_info = {
        .commandPool        = Renderer::command_pool,
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1U
    };

    m_command_buffer = m_device->GetLogicalDevice().allocateCommandBuffers(allocate_info).value.front();

    vk::FenceCreateInfo fence_create_info = {
        .flags = vk::FenceCreateFlagBits::eSignaled
    };

    m_fence = m_device->GetLogicalDevice().createFence(fence_create_info).value;

    vk::SemaphoreCreateInfo semaphore_create_info = {

    };

    m_image_semaphore   = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;
    m_present_semaphore = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;
}

RenderFrame::~RenderFrame() noexcept
{
    
}
