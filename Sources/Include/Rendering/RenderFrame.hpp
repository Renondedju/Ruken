
#pragma once

#include <memory>
#include <vector>

#include "Vulkan/FencePool.hpp"
#include "Vulkan/SemaphorePool.hpp"
#include "Vulkan/CommandPool.hpp"

#include "Rendering/RenderView.hpp"
#include "Rendering/RenderTarget.hpp"

BEGIN_RUKEN_NAMESPACE

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
        RkBool Reset() noexcept;

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

END_RUKEN_NAMESPACE