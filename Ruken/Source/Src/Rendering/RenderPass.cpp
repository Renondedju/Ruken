#include "Rendering/RenderPass.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

RenderPass::RenderPass(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    
}

RenderPass::~RenderPass() noexcept
{
    if (m_handle)
        m_device->GetLogicalDevice().destroy(m_handle);
}

RkVoid RenderPass::Begin(vk::CommandBuffer const& in_command_buffer, RenderFrame& in_frame) const noexcept
{
    vk::ClearValue clear_color = {
        .color = {
            std::array{0.0F, 0.0F, 0.0F, 1.0F}
        }
    };

    vk::ClearValue clear_stencil = {
        .depthStencil = {1, 0}
    };

    std::array clear_values = {
        clear_color,
        clear_stencil
    };

    vk::RenderPassBeginInfo render_pass_begin_info = {
        .renderPass = m_handle,
        .framebuffer = in_frame.GetFramebuffer(),
        .renderArea = {
            .extent = {
                .width  = in_frame.GetColorTarget().GetExtent().width,
                .height = in_frame.GetColorTarget().GetExtent().height
            }
        },
        .clearValueCount = static_cast<RkUint32>(clear_values.size()),
        .pClearValues = clear_values.data()
    };

    in_command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
}

RkVoid RenderPass::End(vk::CommandBuffer const& in_command_buffer) const noexcept
{
    in_command_buffer.endRenderPass();
}