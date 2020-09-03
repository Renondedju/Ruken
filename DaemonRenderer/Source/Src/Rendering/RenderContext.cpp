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