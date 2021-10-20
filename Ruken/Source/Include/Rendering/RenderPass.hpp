#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;
class RenderFrame;
class RenderGraph;

class RenderPass
{
    protected:

        Logger*       m_logger;
        RenderDevice* m_device;
        RenderGraph*  m_graph;

        std::string m_name;

        std::function<RkVoid(vk::CommandBuffer const&)> m_callback;

    public:

        static vk::RenderPass          g_render_pass;
        static std::array<vk::DescriptorSetLayout, 3> g_descriptor_set_layouts;
        static vk::PipelineLayout      g_pipeline_layout;
        static vk::Pipeline            g_pipeline;

        #pragma region Constructors

        RenderPass(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph, std::string const& in_name) noexcept;

        RenderPass(RenderPass const& in_copy) = delete;
        RenderPass(RenderPass&&      in_move) = delete;

        ~RenderPass() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Execute(vk::CommandBuffer const& in_command_buffer, RenderFrame const& in_frame) const noexcept;

        RkVoid SetCallback(std::function<RkVoid(vk::CommandBuffer const&)>&& in_callback) noexcept;

        #pragma region Inputs

        

        #pragma endregion

        #pragma region Outputs

        RkVoid AddColorOutput(std::string const& in_name) noexcept;

        #pragma endregion

        std::string const& GetName() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderPass& operator=(RenderPass const& in_copy) = delete;
        RenderPass& operator=(RenderPass&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE