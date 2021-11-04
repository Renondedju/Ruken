#pragma once

#include "Rendering/RenderTarget.hpp"

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

        std::vector<RenderTarget*> m_color_inputs;
        std::vector<RenderTarget*> m_color_outputs;

        RenderTarget* m_depth_stencil_input;
        RenderTarget* m_depth_stencil_output;

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

        RkVoid AddColorInput (std::string const& in_name);
        RkVoid AddColorOutput(std::string const& in_name, AttachmentInfo const& in_attachment_info);

        RkVoid SetDepthStencilInput (std::string const& in_name);
        RkVoid SetDepthStencilOutput(std::string const& in_name, AttachmentInfo const& in_attachment_info);

        RkVoid SetCallback(std::function<RkVoid(vk::CommandBuffer const&, RenderFrame const&)>&& in_callback) noexcept;

        vk::RenderPass     const& GetHandle() const noexcept;
        vk::PipelineLayout const& GetLayout() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderPass& operator=(RenderPass const& in_copy) = delete;
        RenderPass& operator=(RenderPass&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE