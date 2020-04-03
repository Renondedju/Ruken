/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Rendering/Renderer.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderContext.hpp"

#include "Vulkan/Fence.hpp"
#include "Vulkan/Queue.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Semaphore.hpp"
#include "Vulkan/Swapchain.hpp"
#include "Vulkan/CommandBuffer.hpp"

#include "Windowing/Window.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

RenderContext::RenderContext(Instance const&    in_instance,
                             Device   const&    in_device,
                             Window&            in_window):
    m_device                {in_device},
    m_queue                 {in_device.GetQueueByFlags(VK_QUEUE_GRAPHICS_BIT)},
    m_active_frame_index    {0u},
    m_is_frame_active       {false},
    m_swapchain             {std::make_unique<Swapchain>(in_instance, in_device, in_window)}
{
    if (!m_swapchain)
        return;

    in_window.on_framebuffer_resized += [this](DAEuint32 const in_width, DAEuint32 const in_height)
    {
        OnFramebufferResized(in_width, in_height);
    };
}

#pragma endregion

#pragma region Methods

DAEvoid RenderContext::OnFramebufferResized(DAEuint32 const in_width,
                                            DAEuint32 const in_height) noexcept
{
    if (m_swapchain->ResizeSwapchain(in_width, in_height))
    {

    }
}

DAEbool RenderContext::BeginFrame() noexcept
{
    if (m_is_frame_active)
    {
        GRenderer->GetLogger()->Error("Frame is still active, please call EndFrame().");
        return false;
    }

    m_is_frame_active = m_swapchain->AcquireNextImage(m_frames[m_active_frame_index]->GetImageAvailableSemaphore(), m_active_frame_index);

    return m_is_frame_active;
}

DAEvoid RenderContext::EndFrame() noexcept
{
    if (!m_is_frame_active)
    {
        GRenderer->GetLogger()->Error("Frame is not active, please call BeginFrame().");
        return;
    }

    m_is_frame_active = false;
}

RenderFrame const& RenderContext::GetActiveFrame() const noexcept
{
    if (!m_is_frame_active)
    {
        // TODO : ERROR!
    }

    return *m_frames[m_active_frame_index];
}

#pragma endregion