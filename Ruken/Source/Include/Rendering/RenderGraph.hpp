#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class RenderGraph
{
    private:

        Logger*       m_logger;
        RenderDevice* m_device;

        std::vector<vk::RenderPass> m_render_passes;

    public:

        RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderGraph(RenderGraph const& in_copy) = delete;
        RenderGraph(RenderGraph&&      in_move) = delete;

        ~RenderGraph() noexcept;

        RkVoid AddPass();
        RkVoid Execute();

        #pragma region Operators

        RenderGraph& operator=(RenderGraph const& in_copy) = delete;
        RenderGraph& operator=(RenderGraph&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE