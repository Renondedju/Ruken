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
#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderTarget.hpp"
#include "Rendering/RenderContext.hpp"

#include "Vulkan/Fence.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/CommandBuffer.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

RenderSystem::RenderSystem(Device* in_device) noexcept :
    m_device    { in_device },
    m_queue     { in_device->GetQueueByFlags(VK_QUEUE_GRAPHICS_BIT, 0u) },
    m_context   { nullptr }
{

}

#pragma endregion

#pragma region Methods

DAEvoid RenderSystem::Render() const noexcept
{
    // ...
    if (m_context && m_context->BeginFrame())
    {
        auto& active_frame = m_context->GetActiveFrame();

        // ...
        auto& command_buffer = active_frame->GetCommandBuffer();

        // ...
        active_frame->GetFence().Wait();

        // ...
        command_buffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        // TODO : Record the draw calls here.

        command_buffer.End();

        // ...
        m_context->EndFrame(m_queue, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, command_buffer);
    }
}

DAEvoid RenderSystem::SetContext(RenderContext* in_context) noexcept
{
    m_context = in_context;

    // TODO : Regenerate pipelines if necessary since the only change we care about is the framebuffer which may change size.
}

#pragma endregion