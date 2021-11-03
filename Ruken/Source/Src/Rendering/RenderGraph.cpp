#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

RenderGraph::RenderGraph(Logger* in_logger, Renderer* in_renderer) noexcept:
    m_logger   {in_logger},
    m_renderer {in_renderer}
{

}

RenderGraph::~RenderGraph() noexcept
{

}

RkVoid RenderGraph::Bake() noexcept
{
    
}

RkVoid RenderGraph::Execute(RenderFrame& in_frame) noexcept
{
    for (auto it = m_render_passes.begin(); it != m_render_passes.end(); ++it)
    {
        it->second->Execute(in_frame);
    }
}

RenderPass& RenderGraph::FindOrAddRenderPass(std::string const& in_name) noexcept
{
    if (m_render_passes.contains(in_name))
        return *m_render_passes.at(in_name);

    m_render_passes[in_name] = std::make_unique<RenderPass>(m_logger, m_renderer);

    return *m_render_passes.at(in_name);
}