#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Renderer;
class RenderFrame;

class RenderPass
{
    private:

        Logger*   m_logger;
        Renderer* m_renderer;

        vk::RenderPass     m_handle;
        vk::PipelineLayout m_pipeline_layout;

        std::vector<vk::Framebuffer> m_framebuffers;

        std::function<RkVoid(vk::CommandBuffer const&, RenderFrame const&)> m_callback;

        #pragma region Methods

        

        #pragma endregion

    public:

        #pragma region Constructors

        RenderPass(Logger* in_logger, Renderer* in_renderer) noexcept;

        RenderPass(RenderPass const& in_copy) = delete;
        RenderPass(RenderPass&&      in_move) = delete;

        ~RenderPass() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Execute(RenderFrame& in_frame) const noexcept;

        RkVoid AddColorOutput(std::string const& in_name);

        RkVoid SetCallback(std::function<RkVoid(vk::CommandBuffer const&, RenderFrame const&)>&& in_callback) noexcept;

        vk::RenderPass const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderPass& operator=(RenderPass const& in_copy) = delete;
        RenderPass& operator=(RenderPass&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE