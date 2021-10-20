#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"
#include "Rendering/Renderer.hpp"

#include "Core/ServiceProvider.hpp"

#include "Windowing/WindowManager.hpp"

#include "Rendering/Resources/Model.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model> g_model;

RenderSystem::RenderSystem(ServiceProvider& in_service_provider) noexcept:
    m_device {in_service_provider.LocateService<Renderer>     ()->GetDevice    ()},
    m_window {in_service_provider.LocateService<WindowManager>()->GetMainWindow()}
{
    m_graph = std::make_unique<RenderGraph>(nullptr, m_device);

    auto& forward_pass = m_graph->AddRenderPass("forward");

    forward_pass.SetCallback([&](vk::CommandBuffer const& in_command_buffer) {
        g_model->Render(in_command_buffer);
    });

    g_model = std::make_unique<Model>(m_device, "Data/viking_room.obj");

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

    g_model.reset();
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