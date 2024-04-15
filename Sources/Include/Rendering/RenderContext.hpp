
#pragma once

#include <memory>

#include "Rendering/RenderFrame.hpp"

#include "Vulkan/Core/VulkanSwapchain.hpp"

BEGIN_RUKEN_NAMESPACE

class Window;
class Renderer;
class Scheduler;

class RenderContext
{
    private:

        #pragma region Members

        RkInt32    m_frame_index       {-1};
        RkBool     m_is_frame_active   {false};

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

        RkBool BeginFrame() noexcept;

        RkVoid EndFrame() noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE