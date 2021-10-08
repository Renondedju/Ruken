#pragma once

#include "Rendering/RenderPass.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;
class RenderFrame;

class RenderGraph
{
    private:

        Logger*       m_logger;
        RenderDevice* m_device;

        std::vector<std::unique_ptr<RenderPass>> m_render_passes;

    public:

        #pragma region Constructors

        RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderGraph(RenderGraph const& in_copy) = delete;
        RenderGraph(RenderGraph&&      in_move) = delete;

        ~RenderGraph() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Execute(RenderFrame& in_frame) noexcept;

        template<RenderPassType T, typename... TArgs> requires std::is_constructible_v<T, TArgs...>
        RkVoid AddRenderPass(TArgs&&... in_args) noexcept
        {
            m_render_passes.emplace_back(std::make_unique<T>(std::forward<TArgs>(in_args)...));
        }

        #pragma endregion

        #pragma region Operators

        RenderGraph& operator=(RenderGraph const& in_copy) = delete;
        RenderGraph& operator=(RenderGraph&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE