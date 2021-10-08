#pragma once

#include "Rendering/RenderPass.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class ForwardPass final : public RenderPass
{
    private:

        vk::DescriptorSetLayout m_descriptor_set_layout;
        vk::PipelineLayout      m_pipeline_layout;
        vk::Pipeline            m_pipeline;

    public:

        static vk::RenderPass          g_render_pass;
        static vk::DescriptorSetLayout g_descriptor_set_layout;
        static vk::PipelineLayout      g_pipeline_layout;
        static vk::Pipeline            g_pipeline;

        ForwardPass(Logger* in_logger, RenderDevice* in_device) noexcept;

        ForwardPass(ForwardPass const& in_copy) = delete;
        ForwardPass(ForwardPass&&      in_move) = delete;

        ~ForwardPass() noexcept override;

        RkVoid Begin(vk::CommandBuffer const& in_command_buffer, RenderFrame& in_frame) const noexcept override;
        RkVoid End  (vk::CommandBuffer const& in_command_buffer) const noexcept override;

        #pragma region Operators

        ForwardPass& operator=(ForwardPass const& in_copy) = delete;
        ForwardPass& operator=(ForwardPass&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE