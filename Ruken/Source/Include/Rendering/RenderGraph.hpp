#pragma once

#include "Rendering/RenderPass.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Renderer;
class RenderFrame;

class RenderGraph
{
    private:

        Logger*   m_logger;
        Renderer* m_renderer;

        vk::DescriptorSetLayout m_frame_descriptor_set_layout;
        vk::DescriptorSetLayout m_camera_descriptor_set_layout;

        std::unordered_map<std::string, std::unique_ptr<RenderPass>> m_render_passes;

    public:

        #pragma region Constructors

        RenderGraph(Logger* in_logger, Renderer* in_renderer) noexcept;

        RenderGraph(RenderGraph const& in_copy) = delete;
        RenderGraph(RenderGraph&&      in_move) = delete;

        ~RenderGraph() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Bake() noexcept;
        RkVoid Execute(RenderFrame& in_frame) noexcept;

        RenderPass& FindOrAddRenderPass(std::string const& in_name) noexcept;

        vk::DescriptorSetLayout const& GetFrameDescriptorSetLayout () const noexcept;
        vk::DescriptorSetLayout const& GetCameraDescriptorSetLayout() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderGraph& operator=(RenderGraph const& in_copy) = delete;
        RenderGraph& operator=(RenderGraph&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE