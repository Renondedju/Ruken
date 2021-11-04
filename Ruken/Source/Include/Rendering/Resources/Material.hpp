#pragma once

#include "Rendering/Resources/Shader.hpp"
#include "Rendering/Resources/Texture.hpp"

#include "Resource/IResource.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;
class RenderFrame;

struct MaterialData
{
    RkUint32 albedo_texture;
    RkUint32 normal_texture;
};

class Material final : public IResource
{
    protected:

        Renderer* m_renderer;

        std::vector<vk::PipelineShaderStageCreateInfo>     m_shader_stages;
        std::vector<vk::VertexInputBindingDescription>     m_vertex_input_binding_descriptions;
        std::vector<vk::VertexInputAttributeDescription>   m_vertex_input_attribute_descriptions;
        std::vector<vk::PipelineColorBlendAttachmentState> m_color_blend_attachment_states;
        std::vector<vk::DynamicState>                      m_dynamic_states;

        vk::PipelineVertexInputStateCreateInfo   m_vertex_input_state   = {};
        vk::PipelineInputAssemblyStateCreateInfo m_input_assembly_state = {};
        vk::PipelineTessellationStateCreateInfo  m_tessellation_state   = {};
        vk::PipelineViewportStateCreateInfo      m_viewport_state       = {};
        vk::PipelineRasterizationStateCreateInfo m_rasterization_state  = {};
        vk::PipelineMultisampleStateCreateInfo   m_multisample_state    = {};
        vk::PipelineDepthStencilStateCreateInfo  m_depth_stencil_state  = {};
        vk::PipelineColorBlendStateCreateInfo    m_color_blend_state    = {};
        vk::PipelineDynamicStateCreateInfo       m_dynamic_state        = {};
        vk::PipelineLayout                       m_pipeline_layout      = {};
        vk::Pipeline                             m_pipeline             = {};

        std::vector<std::unique_ptr<Shader>>  m_shaders;
        std::vector<std::unique_ptr<Texture>> m_textures;

    public:

        #pragma region Operators

        Material(Renderer* in_renderer, std::string_view in_path) noexcept;

        Material(Material const& in_copy) = delete;
        Material(Material&&      in_move) = delete;

        ~Material() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Bind(vk::CommandBuffer const& in_command_buffer) const noexcept;

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        std::vector<vk::PipelineShaderStageCreateInfo> const& GetShaderStages() const noexcept;

        vk::PipelineVertexInputStateCreateInfo   const& GetVertexInputState  () const noexcept;
        vk::PipelineInputAssemblyStateCreateInfo const& GetInputAssemblyState() const noexcept;
        vk::PipelineTessellationStateCreateInfo  const& GetTessellationState () const noexcept;
        vk::PipelineViewportStateCreateInfo      const& GetViewportState     () const noexcept;
        vk::PipelineRasterizationStateCreateInfo const& GetRasterizationState() const noexcept;
        vk::PipelineMultisampleStateCreateInfo   const& GetMultisampleState  () const noexcept;
        vk::PipelineDepthStencilStateCreateInfo  const& GetDepthStencilState () const noexcept;
        vk::PipelineColorBlendStateCreateInfo    const& GetColorBlendState   () const noexcept;
        vk::PipelineDynamicStateCreateInfo       const& GetDynamicState      () const noexcept;
        vk::PipelineLayout                       const& GetPipelineLayout    () const noexcept;
        vk::Pipeline                             const& GetPipeline          () const noexcept;

        #pragma endregion

        #pragma region Operators

        Material& operator=(Material const& in_copy) = delete;
        Material& operator=(Material&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE