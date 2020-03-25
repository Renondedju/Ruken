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
#include "Rendering/RenderTarget.hpp"
#include "Rendering/RenderContext.hpp"


#include "Vulkan/Fence.hpp"
#include "Vulkan/Queue.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Swapchain.hpp"
#include "Vulkan/Semaphore.hpp"
#include "Vulkan/CommandBuffer.hpp"

#include "Windowing/Window.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

RenderContext::RenderContext(Device* in_device,
                             Window* in_window) :
    m_device                { in_device },
    m_window                { in_window },
    m_queue                 { in_device->GetQueueByFlags(VK_QUEUE_GRAPHICS_BIT, 0u) },
    m_active_frame_index    { 0u },
    m_is_frame_active       { false },
    m_swapchain             { std::make_unique<Swapchain>(in_device, in_window) }
{
    for (auto image : m_swapchain->GetImages())
    {
        m_frames.push_back(std::make_unique<RenderFrame>(m_device, std::make_unique<RenderTarget>(m_device, image)));
    }

    // TODO : Replaces with a call to a member function which doesn't work yet.
    in_window->on_framebuffer_resized += [this](DAEuint32 const in_width, DAEuint32 const in_height) { ResizeSwapchain(in_width, in_height); };
}

#pragma endregion

#pragma region Methods

DAEvoid RenderContext::ResizeSwapchain(DAEuint32 const in_width,
                                       DAEuint32 const in_height) noexcept
{
    m_swapchain->ResizeSwapchain(in_width, in_height);

    auto it = m_frames.begin();

    for (auto image : m_swapchain->GetImages())
    {
        if (it != m_frames.end())
        {
            (*it)->UpdateRenderTarget(std::make_unique<RenderTarget>(m_device, image));
        }

        else
        {
            m_frames.push_back(std::make_unique<RenderFrame>(m_device, std::make_unique<RenderTarget>(m_device, image)));
        }

        ++it;
    }
}

DAEbool RenderContext::BeginFrame() noexcept
{
    if (m_is_frame_active)
    {
        GRenderer->GetLogger()->Error("Frame is still active, please call EndFrame()!");

        return false;
    }

    m_is_frame_active = m_swapchain->AcquireNextImage(&m_active_frame_index);

    if (!m_is_frame_active)
    {
        auto const size = m_window->GetFramebufferSize();

        ResizeSwapchain(size.width, size.height);

        m_is_frame_active = m_swapchain->AcquireNextImage(&m_active_frame_index);
    }

    return m_is_frame_active;
}

DAEvoid RenderContext::EndFrame(Queue* in_queue, VkPipelineStageFlags const in_stage, CommandBuffer const& in_command_buffer) noexcept
{
    if (!m_is_frame_active)
    {
        GRenderer->GetLogger()->Error("Frame is not active, please call BeginFrame!");

        return;
    }

    auto const& active_frame = m_frames[m_active_frame_index];

    auto const swapchain                 = m_swapchain->GetHandle();
    auto const command_buffer            = in_command_buffer.GetHandle();
    auto const image_available_semaphore = active_frame->GetImageAvailableSemaphore().GetHandle();
    auto const render_finished_semaphore = active_frame->GetRenderFinishedSemaphore().GetHandle();

    if (in_queue)
    {
        VkSubmitInfo submit_info = {};

        submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount   = 1u;
        submit_info.pWaitSemaphores      = &image_available_semaphore;
        submit_info.pWaitDstStageMask    = &in_stage;
        submit_info.commandBufferCount   = 1u;
        submit_info.pCommandBuffers      = &command_buffer;
        submit_info.signalSemaphoreCount = 1u;
        submit_info.pSignalSemaphores    = &render_finished_semaphore;
            
        if (in_queue->Submit(1u, submit_info, active_frame->GetFence().GetHandle()))
        {
            VkPresentInfoKHR present_info = {};

            present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info.waitSemaphoreCount = 1u;
            present_info.pWaitSemaphores    = &render_finished_semaphore;
            present_info.swapchainCount     = 1u;
            present_info.pSwapchains        = &swapchain;
            present_info.pImageIndices      = &m_active_frame_index;

            if (!m_queue->Present(present_info))
            {
                auto const size = m_window->GetFramebufferSize();

                ResizeSwapchain(size.width, size.height);
            }
        }
    }

    m_is_frame_active = false;
}

std::unique_ptr<RenderFrame> const& RenderContext::GetActiveFrame() const noexcept
{
    if (!m_is_frame_active)
    {
        // TODO : ERROR!

        return nullptr;
    }

    return m_frames[m_active_frame_index];
}

#pragma endregion