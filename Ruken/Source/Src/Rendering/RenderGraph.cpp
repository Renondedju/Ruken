#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/Renderer.hpp"

#include "Rendering/Resources/Model.hpp"

#include "Rendering/Passes/ForwardPass.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model> g_model;

RenderGraph::RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    AddRenderPass<ForwardPass>(in_logger, in_device);

    g_model = std::make_unique<Model>(m_device, "Data/viking_room.obj");
}

RenderGraph::~RenderGraph() noexcept
{
    g_model.reset();
}

RkVoid RenderGraph::Execute(RenderFrame& in_frame) noexcept
{
    for (auto const& render_pass: m_render_passes)
    {
        auto command_buffer = in_frame.GetGraphicsCommandPool().Request();

        vk::CommandBufferBeginInfo command_buffer_begin_info = {};

        if (command_buffer.begin(command_buffer_begin_info) != vk::Result::eSuccess)
            continue;

        vk::Viewport viewport = {
            .width    = 1920.0F,
            .height   = 1080.0F,
            .minDepth = 0.0F,
            .maxDepth = 1.0F
        };

        vk::Rect2D scissor = {
            .extent = {
                1920U,
                1080U
            }
        };

        command_buffer.setViewport(0, viewport);
        command_buffer.setScissor (0, scissor);

        render_pass->Begin(command_buffer, in_frame);

        g_model->Render(command_buffer);

        render_pass->End(command_buffer);

        if (command_buffer.end() != vk::Result::eSuccess)
            continue;

        vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
            .commandBuffer = command_buffer
        };

        vk::SemaphoreSubmitInfoKHR timeline_semaphore_submit_info = {
            .semaphore = in_frame.GetTimelineSemaphore(),
            .value     = in_frame.IncrementTimelineSemaphoreValue()
        };

        vk::SubmitInfo2KHR submit_info = {
            .commandBufferInfoCount   = 1U,
            .pCommandBufferInfos      = &command_buffer_submit_info,
            .signalSemaphoreInfoCount = 1U,
            .pSignalSemaphoreInfos    = &timeline_semaphore_submit_info
        };

        m_device->GetGraphicsQueue().Submit(submit_info);

        in_frame.GetGraphicsCommandPool().Release(std::move(command_buffer));
    }
}