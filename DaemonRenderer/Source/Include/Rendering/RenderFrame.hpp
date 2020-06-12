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
#include <vector>

#include "Vulkan/FencePool.hpp"
#include "Vulkan/SemaphorePool.hpp"
#include "Vulkan/CommandPool.hpp"

#include "Rendering/RenderView.hpp"
#include "Rendering/RenderTarget.hpp"

BEGIN_DAEMON_NAMESPACE

class Renderer;
class Scheduler;

class RenderFrame
{
    private:

        #pragma region Members

        std::unique_ptr<FencePool>     m_fence_pool;
        std::unique_ptr<SemaphorePool> m_semaphore_pool;
        std::unique_ptr<CommandPool>   m_graphics_command_pool;
        std::unique_ptr<CommandPool>   m_compute_command_pool;
        std::unique_ptr<RenderTarget>  m_render_target;
        std::vector    <RenderView>    m_render_views;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderFrame(Renderer& in_renderer, Scheduler& in_scheduler) noexcept;

        RenderFrame(RenderFrame const& in_copy) = delete;
        RenderFrame(RenderFrame&&      in_move) noexcept;

        ~RenderFrame() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        DAEbool Reset() noexcept;

        [[nodiscard]] FencePool&     GetFencePool          () const noexcept;
        [[nodiscard]] SemaphorePool& GetSemaphorePool      () const noexcept;
        [[nodiscard]] CommandPool&   GetGraphicsCommandPool() const noexcept;
        [[nodiscard]] CommandPool&   GetComputeCommandPool () const noexcept;
        [[nodiscard]] RenderTarget&  GetRenderTarget       () const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderFrame& operator=(RenderFrame const& in_copy) = delete;
        RenderFrame& operator=(RenderFrame&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE