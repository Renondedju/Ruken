#include "Rendering/Resources/Material.hpp"
#include "Rendering/Resources/Model.hpp"

#include "Rendering/Renderer.hpp"
#include "Rendering/RenderPass.hpp"

#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

Material::Material(Renderer* in_renderer, std::string_view const in_path) noexcept:
    m_renderer {in_renderer}
{
    (void)in_path;

    m_shaders.emplace_back(std::make_unique<Shader>(m_renderer->GetDevice(), "Data/vert.spv"));
    m_shaders.emplace_back(std::make_unique<Shader>(m_renderer->GetDevice(), "Data/frag.spv"));

    // TODO : Shaders' reflection.

    std::vector descriptor_set_layouts = {
        m_renderer->GetGraph()->GetTextureDescriptorSetLayout(),
        m_renderer->GetGraph()->GetFrameDescriptorSetLayout(),
        m_renderer->GetGraph()->GetCameraDescriptorSetLayout()
    };

    vk::PushConstantRange push_constant_range = {
        .stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
        .size       = sizeof(RkUint32)
    };

    vk::PipelineLayoutCreateInfo layout_info = {
        .setLayoutCount         = static_cast<RkUint32>(descriptor_set_layouts.size()),
        .pSetLayouts            = descriptor_set_layouts.data(),
        .pushConstantRangeCount = 1U,
        .pPushConstantRanges    = &push_constant_range
    };

    m_pipeline_layout = m_renderer->GetDevice()->GetLogicalDevice().createPipelineLayout(layout_info).value;

    m_shader_stages.emplace_back(m_shaders[0]->GetShaderStage());
    m_shader_stages.emplace_back(m_shaders[1]->GetShaderStage());

    m_shader_stages[0].stage = vk::ShaderStageFlagBits::eVertex;
    m_shader_stages[1].stage = vk::ShaderStageFlagBits::eFragment;

    m_vertex_input_binding_descriptions.push_back({
        .binding   = 0U,
        .stride    = sizeof(Vertex),
        .inputRate = vk::VertexInputRate::eVertex
    });

    m_vertex_input_attribute_descriptions.push_back({
        .location = 0,
        .binding = 0,
        .format = vk::Format::eR32G32B32Sfloat,
        .offset = offsetof(Vertex, position)
    });

    m_vertex_input_attribute_descriptions.push_back({
        .location = 1,
        .binding = 0,
        .format = vk::Format::eR32G32B32Sfloat,
        .offset = offsetof(Vertex, color)
    });

    m_vertex_input_attribute_descriptions.push_back({
        .location = 2,
        .binding = 0,
        .format = vk::Format::eR32G32Sfloat,
        .offset = offsetof(Vertex, uv)
    });

    m_vertex_input_state.vertexBindingDescriptionCount   = static_cast<RkUint32>(m_vertex_input_binding_descriptions.size());
    m_vertex_input_state.pVertexBindingDescriptions      = m_vertex_input_binding_descriptions.data();
    m_vertex_input_state.vertexAttributeDescriptionCount = static_cast<RkUint32>(m_vertex_input_attribute_descriptions.size());
    m_vertex_input_state.pVertexAttributeDescriptions    = m_vertex_input_attribute_descriptions.data();

    m_input_assembly_state.topology               = vk::PrimitiveTopology::eTriangleList;
    m_input_assembly_state.primitiveRestartEnable = VK_FALSE;

    m_viewport_state.viewportCount = 1U;
    m_viewport_state.scissorCount  = 1U;

    m_rasterization_state.polygonMode = vk::PolygonMode::eFill;
    m_rasterization_state.cullMode    = vk::CullModeFlagBits::eBack;
    m_rasterization_state.frontFace   = vk::FrontFace::eCounterClockwise;
    m_rasterization_state.lineWidth   = 1.0F;

    m_depth_stencil_state.depthTestEnable       = VK_TRUE;
    m_depth_stencil_state.depthWriteEnable      = VK_TRUE;
    m_depth_stencil_state.depthCompareOp        = vk::CompareOp::eLess;
    m_depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
    m_depth_stencil_state.stencilTestEnable     = VK_FALSE;

    vk::PipelineColorBlendAttachmentState color_blend_attachment = {
        .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eA
    };

    m_color_blend_attachment_states.emplace_back(color_blend_attachment);

    m_color_blend_state.attachmentCount = static_cast<RkUint32>(m_color_blend_attachment_states.size());
    m_color_blend_state.pAttachments    = m_color_blend_attachment_states.data();

    m_dynamic_states.emplace_back(vk::DynamicState::eViewport);
    m_dynamic_states.emplace_back(vk::DynamicState::eScissor);

    m_dynamic_state.dynamicStateCount = static_cast<RkUint32>(m_dynamic_states.size());
    m_dynamic_state.pDynamicStates    = m_dynamic_states.data();

    vk::GraphicsPipelineCreateInfo pipeline_info = {
        .stageCount          = static_cast<RkUint32>(m_shader_stages.size()),
        .pStages             = m_shader_stages.data(),
        .pVertexInputState   = &m_vertex_input_state,
        .pInputAssemblyState = &m_input_assembly_state,
        .pViewportState      = &m_viewport_state,
        .pRasterizationState = &m_rasterization_state,
        .pMultisampleState   = &m_multisample_state,
        .pDepthStencilState  = &m_depth_stencil_state,
        .pColorBlendState    = &m_color_blend_state,
        .pDynamicState       = &m_dynamic_state,
        .layout              = m_pipeline_layout,
        .renderPass          = m_renderer->GetGraph()->FindPass("Forward")->GetHandle()
    };

    m_pipeline = m_renderer->GetDevice()->GetLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info).value;
}

Material::~Material() noexcept
{
    if (m_pipeline_layout)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_pipeline_layout);
    if (m_pipeline)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_pipeline);
}

RkVoid Material::Bind(vk::CommandBuffer const& in_command_buffer) const noexcept
{
    in_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline);
}

RkVoid Material::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid Material::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid Material::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}

std::vector<vk::PipelineShaderStageCreateInfo> const& Material::GetShaderStages() const noexcept
{
    return m_shader_stages;
}

vk::PipelineVertexInputStateCreateInfo const& Material::GetVertexInputState() const noexcept
{
    return m_vertex_input_state;
}

vk::PipelineInputAssemblyStateCreateInfo const& Material::GetInputAssemblyState() const noexcept
{
    return m_input_assembly_state;
}

vk::PipelineTessellationStateCreateInfo const& Material::GetTessellationState() const noexcept
{
    return m_tessellation_state;
}

vk::PipelineViewportStateCreateInfo const& Material::GetViewportState() const noexcept
{
    return m_viewport_state;
}

vk::PipelineRasterizationStateCreateInfo const& Material::GetRasterizationState() const noexcept
{
    return m_rasterization_state;
}

vk::PipelineMultisampleStateCreateInfo const& Material::GetMultisampleState() const noexcept
{
    return m_multisample_state;
}

vk::PipelineDepthStencilStateCreateInfo const& Material::GetDepthStencilState() const noexcept
{
    return m_depth_stencil_state;
}

vk::PipelineColorBlendStateCreateInfo const& Material::GetColorBlendState() const noexcept
{
    return m_color_blend_state;
}

vk::PipelineDynamicStateCreateInfo const& Material::GetDynamicState() const noexcept
{
    return m_dynamic_state;
}

vk::PipelineLayout const& Material::GetPipelineLayout() const noexcept
{
    return m_pipeline_layout;
}

vk::Pipeline const& Material::GetPipeline() const noexcept
{
    return m_pipeline;
}