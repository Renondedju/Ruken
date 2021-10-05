#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Renderer.hpp"

#include "Rendering/Resources/Model.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model> g_model;

RenderSystem::RenderSystem(RenderDevice* in_device, Renderer* in_renderer) noexcept:
    m_device {in_device},
    m_renderer {in_renderer}
{
    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_frames.emplace_back(std::make_unique<RenderFrame>(nullptr, m_device));
    }

    g_model = std::make_unique<Model>(m_device, "Data/viking_room.obj");
}

RenderSystem::~RenderSystem() noexcept
{
    g_model.reset();
}

RkVoid RenderSystem::Render()
{
    m_current_frame = (m_current_frame + 1) % 2;

    auto& frame = *m_frames[m_current_frame];

    frame.Reset();

    auto const& command_buffer = frame.RequestCommandBuffer();

    vk::CommandBufferBeginInfo command_buffer_begin_info = {

    };

    command_buffer.begin(command_buffer_begin_info);

    vk::Viewport viewport = {
        .width    = 1920.0f,
        .height   = 1080.0f,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    vk::Rect2D scissor = {
        .extent = {
            1920,
            1080
        }
    };

    command_buffer.setViewport(0, viewport);
    command_buffer.setScissor (0, scissor);

    vk::ClearValue clear_color = { .color = {std::array{0.0f, 0.0f, 0.0f, 1.0f}}};

    vk::ClearValue clear_stencil { .depthStencil = {1, 0} };

    std::vector clear_values = {
        clear_color, clear_stencil
    };

    vk::RenderPassBeginInfo render_pass_begin_info = {
        .renderPass = Renderer::render_pass,
        .framebuffer = frame.GetFramebuffer(),
        .renderArea = {
            .extent = {
                .width  = frame.GetColorTarget().GetExtent().width,
                .height = frame.GetColorTarget().GetExtent().height
            }
        },
        .clearValueCount = static_cast<uint32_t>(clear_values.size()),
        .pClearValues = clear_values.data()
    };

    command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

    command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, Renderer::pipeline);

    g_model->Render(command_buffer);

    command_buffer.endRenderPass();

    command_buffer.end();

    vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
        .commandBuffer = command_buffer
    };

    vk::SemaphoreSubmitInfoKHR timeline_semaphore_submit_info = {
        .semaphore = frame.GetTimelineSemaphore(),
        .value     = frame.IncrementTimelineSemaphoreValue()
    };

    vk::SubmitInfo2KHR submit_info = {
        .commandBufferInfoCount   = 1U,
        .pCommandBufferInfos      = &command_buffer_submit_info,
        .signalSemaphoreInfoCount = 1U,
        .pSignalSemaphoreInfos    = &timeline_semaphore_submit_info
    };

    m_device->GetGraphicsQueue().Submit(submit_info);

    m_renderer->GetMainWindow().Present(frame);
}