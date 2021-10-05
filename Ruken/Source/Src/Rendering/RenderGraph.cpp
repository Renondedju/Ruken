#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

RenderGraph::RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    
}

RenderGraph::~RenderGraph() noexcept
{
    
}

RkVoid RenderGraph::AddPass()
{
    
}

RkVoid RenderGraph::Execute()
{
    
}