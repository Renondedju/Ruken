#pragma once

#include "Rendering/RenderPass.hpp"

#include "Rendering/RenderObjects/Image.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Renderer;
class RenderFrame;

class RenderGraph
{
    private:

        Logger*       m_logger;
        RenderDevice* m_device;

        vk::DescriptorSetLayout m_texture_descriptor_set_layout;
        vk::DescriptorSetLayout m_frame_descriptor_set_layout;
        vk::DescriptorSetLayout m_camera_descriptor_set_layout;
        vk::PipelineLayout      m_default_pipeline_layout;

        Image const* m_final_target {nullptr};
        RkUint32     m_frame_index  {0U};

        std::vector<std::unique_ptr<RenderFrame>> m_frames;

        std::unordered_map<std::string, std::unique_ptr<RenderPass>>     m_passes;
        std::unordered_map<std::string, std::unique_ptr<RenderResource>> m_resources;

    public:

        static std::unique_ptr<Image> g_color_target;
        static std::unique_ptr<Image> g_depth_target;

        #pragma region Constructors

        RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderGraph(RenderGraph const& in_copy) = delete;
        RenderGraph(RenderGraph&&      in_move) = delete;

        ~RenderGraph() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Build  () noexcept;
        RkVoid Execute(vk::Semaphore const& in_wait_semaphore, vk::Semaphore const& in_signal_semaphore) noexcept;

        RenderPass& AddPass (std::string const& in_name) noexcept;
        RenderPass* FindPass(std::string const& in_name) const noexcept;

        RkVoid SetFinalTarget(Image const& in_target) noexcept;

        RenderFrame& BeginFrame     () noexcept;
        RenderFrame& GetCurrentFrame() noexcept;
        RkVoid       EndFrame       () noexcept;

        RenderImageResource&  GetImageResource (std::string const& in_name) noexcept;
        RenderBufferResource& GetBufferResource(std::string const& in_name) noexcept;

        vk::DescriptorSetLayout const& GetTextureDescriptorSetLayout() const noexcept;
        vk::DescriptorSetLayout const& GetFrameDescriptorSetLayout  () const noexcept;
        vk::DescriptorSetLayout const& GetCameraDescriptorSetLayout () const noexcept;
        vk::PipelineLayout      const& GetDefaultPipelineLayout     () const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderGraph& operator=(RenderGraph const& in_copy) = delete;
        RenderGraph& operator=(RenderGraph&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE