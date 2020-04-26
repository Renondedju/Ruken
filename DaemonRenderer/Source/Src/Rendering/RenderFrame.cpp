/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Rendering/RenderFrame.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

RenderFrame::RenderFrame(Scheduler& in_scheduler) noexcept:
    m_fence_pool        {std::make_unique<FencePool>    ()},
    m_semaphore_pool    {std::make_unique<SemaphorePool>()}
{
    m_command_pool = std::make_unique<CommandPool>(in_scheduler, 0u);
}

RenderFrame::RenderFrame(RenderFrame&& in_move) noexcept:
    m_fence_pool        {std::move(in_move.m_fence_pool)},
    m_semaphore_pool    {std::move(in_move.m_semaphore_pool)},
    m_command_pool      {std::move(in_move.m_command_pool)},
    m_render_views      {std::move(in_move.m_render_views)}
{

}

#pragma endregion

#pragma region Methods

DAEvoid RenderFrame::Reset() noexcept
{
    m_fence_pool    ->Reset();
    m_semaphore_pool->Reset();
    m_command_pool  ->Reset();

    m_render_views.clear();
}

VulkanFence& RenderFrame::RequestFence() const noexcept
{
    return m_fence_pool->RequestFence();
}

VulkanSemaphore& RenderFrame::RequestSemaphore() const noexcept
{
    return m_semaphore_pool->RequestSemaphore();
}

VulkanTimelineSemaphore& RenderFrame::RequestTimelineSemaphore() const noexcept
{
    return m_semaphore_pool->RequestTimelineSemaphore();
}

VulkanCommandBuffer* RenderFrame::RequestCommandBuffer(VkCommandBufferLevel const in_level) const noexcept
{
    return m_command_pool->RequestCommandBuffer(in_level);
}

RenderTarget const& RenderFrame::GetRenderTarget() const noexcept
{
    return *m_render_target;
}

#pragma endregion