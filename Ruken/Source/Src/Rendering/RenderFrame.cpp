#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    m_color_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eR8G8B8A8Unorm,  vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc);
    m_depth_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eD24UnormS8Uint, vk::ImageUsageFlagBits::eDepthStencilAttachment);

    std::array attachments = {
        m_color_target->GetImageView(),
        m_depth_target->GetImageView()
    };

    vk::FramebufferCreateInfo framebuffer_create_info = {
        .renderPass      = Renderer::render_pass,
        .attachmentCount = static_cast<RkUint32>(attachments.size()),
        .pAttachments    = attachments.data(),
        .width           = 1920,
        .height          = 1080,
        .layers          = 1
    };

    m_framebuffer = m_device->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value;

    vk::CommandPoolCreateInfo command_pool_create_info = {
        .queueFamilyIndex = m_device->GetGraphicsFamilyIndex()
    };

    m_command_pool = m_device->GetLogicalDevice().createCommandPool(command_pool_create_info).value;

    vk::CommandBufferAllocateInfo command_buffer_allocate_info = {
        .commandPool        = m_command_pool,
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 4U
    };

    m_command_buffers = m_device->GetLogicalDevice().allocateCommandBuffers(command_buffer_allocate_info).value;

    vk::SemaphoreTypeCreateInfo semaphore_type_create_info = {
        .semaphoreType = vk::SemaphoreType::eTimeline,
        .initialValue  = m_semaphore_value
    };

    vk::SemaphoreCreateInfo semaphore_create_info = {
        .pNext = &semaphore_type_create_info
    };

    m_timeline_semaphore = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;

    semaphore_type_create_info.semaphoreType = vk::SemaphoreType::eBinary;

    m_image_semaphore   = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;
    m_present_semaphore = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;
}

RenderFrame::~RenderFrame() noexcept
{
    m_color_target.reset();
    m_depth_target.reset();

    m_device->GetLogicalDevice().destroy(m_framebuffer);
    m_device->GetLogicalDevice().destroy(m_command_pool);
    m_device->GetLogicalDevice().destroy(m_timeline_semaphore);
    m_device->GetLogicalDevice().destroy(m_image_semaphore);
    m_device->GetLogicalDevice().destroy(m_present_semaphore);
}

RkVoid RenderFrame::Reset() noexcept
{
    vk::SemaphoreWaitInfo semaphore_wait_info = {
        .semaphoreCount = 1U,
        .pSemaphores    = &m_timeline_semaphore,
        .pValues        = &m_semaphore_value
    };

    m_device->GetLogicalDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX);

    m_device->GetLogicalDevice().resetCommandPool(m_command_pool);

    m_command_buffer_index = 0U;
}

RkUint64 RenderFrame::IncrementTimelineSemaphoreValue() noexcept
{
    return ++m_semaphore_value;
}

vk::CommandBuffer const& RenderFrame::RequestCommandBuffer() noexcept
{
    return m_command_buffers[m_command_buffer_index++];
}

RenderTarget const& RenderFrame::GetColorTarget() const noexcept
{
    return *m_color_target;
}

RenderTarget const& RenderFrame::GetDepthTarget() const noexcept
{
    return *m_depth_target;
}

vk::Framebuffer const& RenderFrame::GetFramebuffer() const noexcept
{
    return m_framebuffer;
}

RkUint64 const& RenderFrame::GetTimelineSemaphoreValue() const noexcept
{
    return m_semaphore_value;
}

vk::Semaphore const& RenderFrame::GetTimelineSemaphore() const noexcept
{
    return m_timeline_semaphore;
}

vk::Semaphore const& RenderFrame::GetImageSemaphore() const noexcept
{
    return m_image_semaphore;
}

vk::Semaphore const& RenderFrame::GetPresentSemaphore() const noexcept
{
    return m_present_semaphore;
}
}
