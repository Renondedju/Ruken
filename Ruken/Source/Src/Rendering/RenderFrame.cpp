#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Rendering/Passes/ForwardPass.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger                {in_logger},
    m_device                {in_device},
    m_graphics_command_pool {in_device, {}, in_device->GetGraphicsFamilyIndex()},
    m_compute_command_pool  {in_device, {}, in_device->GetComputeFamilyIndex ()},
    m_transfer_command_pool {in_device, {}, in_device->GetTransferFamilyIndex()},
    m_semaphore_pool        {in_device}
{
    m_color_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eR8G8B8A8Unorm,  vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc);
    m_depth_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eD24UnormS8Uint, vk::ImageUsageFlagBits::eDepthStencilAttachment);

    std::array attachments = {
        m_color_target->GetImageView(),
        m_depth_target->GetImageView()
    };

    vk::FramebufferCreateInfo framebuffer_create_info = {
        .renderPass      = ForwardPass::g_render_pass,
        .attachmentCount = static_cast<RkUint32>(attachments.size()),
        .pAttachments    = attachments.data(),
        .width           = 1920,
        .height          = 1080,
        .layers          = 1
    };

    m_framebuffer = m_device->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value;

    vk::SemaphoreTypeCreateInfo semaphore_type_create_info = {
        .semaphoreType = vk::SemaphoreType::eTimeline,
        .initialValue  = m_semaphore_value
    };

    vk::SemaphoreCreateInfo semaphore_create_info = {
        .pNext = &semaphore_type_create_info
    };

    m_timeline_semaphore = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info).value;
}

RenderFrame::~RenderFrame() noexcept
{
    m_color_target.reset();
    m_depth_target.reset();

    m_device->GetLogicalDevice().destroy(m_framebuffer);
    m_device->GetLogicalDevice().destroy(m_timeline_semaphore);
}

RkVoid RenderFrame::Reset() noexcept
{
    vk::SemaphoreWaitInfo semaphore_wait_info = {
        .semaphoreCount = 1U,
        .pSemaphores    = &m_timeline_semaphore,
        .pValues        = &m_semaphore_value
    };

    m_device->GetLogicalDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX);

    m_graphics_command_pool.Reset();
    m_compute_command_pool .Reset();
    m_transfer_command_pool.Reset();
    m_semaphore_pool       .Reset();
}

RkUint64 RenderFrame::IncrementTimelineSemaphoreValue() noexcept
{
    return ++m_semaphore_value;
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

CommandPool& RenderFrame::GetGraphicsCommandPool() noexcept
{
    return m_graphics_command_pool;
}

CommandPool& RenderFrame::GetComputeCommandPool() noexcept
{
    return m_compute_command_pool;
}

CommandPool& RenderFrame::GetTransferCommandPool() noexcept
{
    return m_transfer_command_pool;
}

SemaphorePool& RenderFrame::GetSemaphorePool() noexcept
{
    return m_semaphore_pool;
}
}
