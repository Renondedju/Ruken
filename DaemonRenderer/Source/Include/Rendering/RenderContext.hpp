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

#pragma once

#include <memory>
#include <vector>

#include "Types/FundamentalTypes.hpp"

#include "Vulkan/Swapchain.hpp"

BEGIN_DAEMON_NAMESPACE

class Queue;
class Device;
class Window;
class Semaphore;
class RenderFrame;
class CommandBuffer;

class RenderContext
{
    private:

        #pragma region Members

        Device const&                               m_device;
        Queue  const&                               m_queue;
        DAEuint32                                   m_active_frame_index;
        DAEbool                                     m_is_frame_active;
        std::unique_ptr<Swapchain>                  m_swapchain;
        std::vector<std::unique_ptr<RenderFrame>>   m_frames;

        #pragma endregion

        #pragma region Methods

        DAEvoid OnFramebufferResized(DAEuint32 in_width, DAEuint32 in_height) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit RenderContext(Instance const&  in_instance,
                               Device   const&  in_device,
                               Window&          in_window);

        RenderContext(RenderContext const&  in_copy) = delete;
        RenderContext(RenderContext&&       in_move) = delete;

        ~RenderContext() = default;

        #pragma endregion

        #pragma region Operators

        RenderContext& operator=(RenderContext const&   in_copy) = delete;
        RenderContext& operator=(RenderContext&&        in_move) = delete;

        #pragma endregion

        #pragma region Methods

        DAEbool BeginFrame() noexcept;

        DAEvoid EndFrame() noexcept;

        [[nodiscard]]
        std::vector<std::unique_ptr<RenderFrame>> const& GetFrames() const noexcept;

        [[nodiscard]]
        RenderFrame const& GetActiveFrame() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE