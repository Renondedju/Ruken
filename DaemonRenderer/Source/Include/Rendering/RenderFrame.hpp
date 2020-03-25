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

#include "Vulkan/Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Fence;
class Device;
class Semaphore;
class CommandPool;
class RenderTarget;
class CommandBuffer;

class RenderFrame
{
    private:

        #pragma region Members

        Device*         m_device;

        std::unique_ptr<RenderTarget>   m_render_target;
        std::unique_ptr<Fence>          m_fence;
        std::unique_ptr<Semaphore>      m_image_available_semaphore;
        std::unique_ptr<Semaphore>      m_render_finished_semaphore;

        #pragma endregion

        #pragma region Methods

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderFrame() = delete;

        explicit RenderFrame(Device* in_device, std::unique_ptr<RenderTarget>&& in_render_target) noexcept;

        RenderFrame(RenderFrame const&  in_copy) = delete;
        RenderFrame(RenderFrame&&       in_move) = delete;

        ~RenderFrame() = default;

        #pragma endregion

        #pragma region Operators

        RenderFrame& operator=(RenderFrame const&   in_copy) = delete;
        RenderFrame& operator=(RenderFrame&&        in_move) = delete;

        #pragma endregion

        #pragma region Methods

        DAEvoid UpdateRenderTarget(std::unique_ptr<RenderTarget>&& in_render_target);

        CommandBuffer const& GetCommandBuffer() const noexcept;

        RenderTarget const& GetRenderTarget() const noexcept;

        Fence const& GetFence() const noexcept;

        Semaphore const& GetImageAvailableSemaphore() const noexcept;

        Semaphore const& GetRenderFinishedSemaphore() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE