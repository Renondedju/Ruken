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

#pragma once

#include <memory>

#include "Rendering/RenderFrame.hpp"

#include "Vulkan/Core/VulkanSwapchain.hpp"

BEGIN_DAEMON_NAMESPACE

class Window;
class Renderer;
class Scheduler;

class RenderContext
{
    private:

        #pragma region Members

        DAEint32    m_frame_index       {-1};
        DAEbool     m_is_frame_active   {false};

        std::unique_ptr<VulkanSwapchain> m_swapchain;

        std::vector<RenderFrame> m_render_frames;

        #pragma endregion

        #pragma region Methods



        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit RenderContext(Renderer&  in_renderer,
                               Scheduler& in_scheduler,
                               Window&    in_window);

        RenderContext(RenderContext const& in_copy) = delete;
        RenderContext(RenderContext&&      in_move) = default;

        ~RenderContext() = default;

        #pragma endregion

        #pragma region Operators

        RenderContext& operator=(RenderContext const&   in_copy) = delete;
        RenderContext& operator=(RenderContext&&        in_move) = delete;

        #pragma endregion

        #pragma region Methods

        DAEbool BeginFrame() noexcept;

        DAEvoid EndFrame() noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE