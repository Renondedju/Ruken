#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"
#include "Rendering/Renderer.hpp"

#include "Core/ServiceProvider.hpp"

#include "Windowing/WindowManager.hpp"

USING_RUKEN_NAMESPACE

RenderSystem::RenderSystem(ServiceProvider& in_service_provider) noexcept:
    m_device {in_service_provider.LocateService<Renderer>     ()->GetDevice    ()},
    m_window {in_service_provider.LocateService<WindowManager>()->GetMainWindow()}
{
    m_graph = std::make_unique<RenderGraph>(nullptr, m_device);

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_frames.emplace_back(std::make_unique<RenderFrame>(nullptr, m_device));
    }
}

RenderSystem::~RenderSystem() noexcept
{
    if (m_device->GetLogicalDevice().waitIdle() != vk::Result::eSuccess)
    {
        
    }

    m_graph.reset();
}

RkVoid RenderSystem::Update() noexcept
{
    m_current_frame = (m_current_frame + 1) % 2;

    auto& frame = *m_frames[m_current_frame];

    frame.Reset();

    m_graph ->Execute(frame);
    m_window->Present(frame);
}