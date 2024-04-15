
#include "Rendering/RenderContext.hpp"
#include "Rendering/Renderer.hpp"

#include "Windowing/Window.hpp"

#include "Vulkan/Core/VulkanSwapchain.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor

RenderContext::RenderContext(Renderer&  in_renderer,
                             Scheduler& in_scheduler,
                             Window&    in_window):
    m_swapchain {std::make_unique<VulkanSwapchain>(in_renderer.GetPhysicalDevice(), in_renderer.GetDevice(), in_window)}
{
    for (RkUint32 i = 0; i < 2u; ++i)
        m_render_frames.emplace_back(in_renderer, in_scheduler);
}

#pragma endregion

#pragma region Methods

RkBool RenderContext::BeginFrame() noexcept
{
    if (m_is_frame_active)
        return false;

    m_frame_index = m_frame_index + 1 % m_render_frames.size();

    auto& active_frame = m_render_frames[m_frame_index];

    if (!active_frame.Reset())
        return false;

    m_is_frame_active = true;

    return true;
}

RkVoid RenderContext::EndFrame() noexcept
{
    if (!m_is_frame_active)
        return;

    if (m_swapchain->IsValid())
        m_swapchain->Present(m_render_frames[m_frame_index]);

    m_is_frame_active = false;
}

#pragma endregion