#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

RenderGraph::RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{

}

RenderGraph::~RenderGraph() noexcept
{

}

RkVoid RenderGraph::Bake() noexcept
{
    for (auto const& render_pass: m_render_passes)
    {
        (void)render_pass;
    }
}

RkVoid RenderGraph::Execute(RenderFrame& in_frame) noexcept
{
    auto command_buffers = in_frame.GetGraphicsCommandPool().Request(m_render_passes.size());

    for (RkSize i = 0; i < m_render_passes.size(); ++i)
    {
        vk::CommandBufferBeginInfo command_buffer_begin_info = {};

        if (command_buffers[i].begin(command_buffer_begin_info) != vk::Result::eSuccess)
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

        command_buffers[i].setViewport(0, viewport);
        command_buffers[i].setScissor (0, scissor);

        m_render_passes[i]->Execute(command_buffers[i], in_frame);

        if (command_buffers[i].end() != vk::Result::eSuccess)
            continue;

        vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
            .commandBuffer = command_buffers[i]
        };

        vk::SemaphoreSubmitInfoKHR timeline_semaphore_submit_info = {
            .semaphore = in_frame.GetTimelineSemaphore().GetHandle(),
            .value     = in_frame.GetTimelineSemaphore().NextValue()
        };

        vk::SubmitInfo2KHR submit_info = {
            .commandBufferInfoCount   = 1U,
            .pCommandBufferInfos      = &command_buffer_submit_info,
            .signalSemaphoreInfoCount = 1U,
            .pSignalSemaphoreInfos    = &timeline_semaphore_submit_info
        };

        m_device->GetGraphicsQueue().Submit(submit_info);
    }

    in_frame.GetGraphicsCommandPool().Release(std::move(command_buffers));
}

RenderPass& RenderGraph::AddRenderPass(std::string const& in_name) noexcept
{
    return *m_render_passes.emplace_back(std::make_unique<RenderPass>(m_logger, m_device, this, in_name));
}

RenderPass* RenderGraph::GetRenderPass(std::string const& in_name) const noexcept
{
    for (auto const& render_pass : m_render_passes)
    {
        if (render_pass->GetName() == in_name)
            return render_pass.get();
    }

    return nullptr;
}