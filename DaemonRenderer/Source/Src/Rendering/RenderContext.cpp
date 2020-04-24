﻿/*
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

#include "Windowing/Window.hpp"

#include "Vulkan/Core/VulkanSwapchain.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

RenderContext::RenderContext(VulkanPhysicalDevice& in_physical_device,
                             VulkanDevice&  in_device,
                             Window&        in_window):
    m_swapchain {std::make_unique<VulkanSwapchain>(in_physical_device, in_device, in_window)}
{
    for (DAEuint32 i = 0; i < 2u; ++i)
        m_render_frames.emplace_back();

    in_window.on_framebuffer_resized += [this] (DAEint32 const in_width, DAEint32 const in_height)
    {
        if (m_swapchain)
            m_swapchain->Resize(in_width, in_height);
    };
}

#pragma endregion

#pragma region Methods

DAEbool RenderContext::BeginFrame() noexcept
{
    if (m_is_frame_active)
        return false;

    m_frame_index = m_frame_index + 1 % m_render_frames.size();

    auto& active_frame = m_render_frames[m_frame_index];

    active_frame.Reset();

    return true;
}

DAEvoid RenderContext::EndFrame() noexcept
{
    if (!m_is_frame_active)
        return;



    m_is_frame_active = false;
}

#pragma endregion