
#include "Rendering/RenderFrame.hpp"

#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

RenderFrame::RenderFrame(Renderer& in_renderer, Scheduler& in_scheduler) noexcept:
    m_fence_pool            {std::make_unique<FencePool>    ()},
    m_semaphore_pool        {std::make_unique<SemaphorePool>()},
    m_graphics_command_pool {std::make_unique<CommandPool>  (in_scheduler, in_renderer.GetDevice().GetGraphicsFamily())},
    m_compute_command_pool  {std::make_unique<CommandPool>  (in_scheduler, in_renderer.GetDevice().GetComputeFamily ())},
    m_render_target         {std::make_unique<RenderTarget> ()}
{

}

RenderFrame::RenderFrame(RenderFrame&& in_move) noexcept:
    m_fence_pool            {std::move(in_move.m_fence_pool)},
    m_semaphore_pool        {std::move(in_move.m_semaphore_pool)},
    m_graphics_command_pool {std::move(in_move.m_graphics_command_pool)},
    m_compute_command_pool  {std::move(in_move.m_compute_command_pool)},
    m_render_target         {std::move(in_move.m_render_target)},
    m_render_views          {std::move(in_move.m_render_views)}
{

}

#pragma endregion

#pragma region Methods

RkBool RenderFrame::Reset() noexcept
{
    m_render_views.clear();

    if (m_fence_pool           ->Reset() &&
        m_semaphore_pool       ->Reset() &&
        m_graphics_command_pool->Reset() &&
        m_compute_command_pool ->Reset() )
        return true;

    return false;
}

FencePool& RenderFrame::GetFencePool() const noexcept
{
    return *m_fence_pool;
}

SemaphorePool& RenderFrame::GetSemaphorePool() const noexcept
{
    return *m_semaphore_pool;
}

CommandPool& RenderFrame::GetGraphicsCommandPool() const noexcept
{
    return *m_graphics_command_pool;
}

CommandPool& RenderFrame::GetComputeCommandPool() const noexcept
{
    return *m_compute_command_pool;
}

RenderTarget& RenderFrame::GetRenderTarget() const noexcept
{
    return *m_render_target;
}

#pragma endregion