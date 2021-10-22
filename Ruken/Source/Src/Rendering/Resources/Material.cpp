#include "Rendering/Resources/Material.hpp"
#include "Rendering/Resources/Model.hpp"

#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderPass.hpp"

#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

Material::Material(RenderDevice* in_device, std::string_view const in_path) noexcept:
    m_device {in_device}
{
    (void)in_path;

    Shader vertex_shader  (m_device, "Data/vert.spv");
    Shader fragment_shader(m_device, "Data/frag.spv");

    std::vector shader_stages = {
        vertex_shader.GetShaderStage(), fragment_shader.GetShaderStage()
    };

    shader_stages[0].stage = vk::ShaderStageFlagBits::eVertex;
    shader_stages[1].stage = vk::ShaderStageFlagBits::eFragment;

    auto binding_description    = Vertex::get_binding_description();
    auto attribute_descriptions = Vertex::get_attribute_descriptions();

    vk::PipelineVertexInputStateCreateInfo vertex_input_state = {
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &binding_description,
        .vertexAttributeDescriptionCount = 3,
        .pVertexAttributeDescriptions = attribute_descriptions.data()
    };

    vk::PipelineInputAssemblyStateCreateInfo input_assembly_state = {
        .topology = vk::PrimitiveTopology::eTriangleList,
        .primitiveRestartEnable = VK_FALSE
    };

    vk::PipelineViewportStateCreateInfo viewport_state = {
        .viewportCount = 1,
        .scissorCount = 1,
    };

    vk::PipelineRasterizationStateCreateInfo rasterization_state = {
        .polygonMode = vk::PolygonMode::eFill,
        .cullMode = vk::CullModeFlagBits::eBack,
        .frontFace = vk::FrontFace::eCounterClockwise,
        .lineWidth = 1.0F
    };

    vk::PipelineMultisampleStateCreateInfo multisample_state = {

    };

    vk::PipelineDepthStencilStateCreateInfo depth_stencil_state = {
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = vk::CompareOp::eLess,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE
    };

    vk::PipelineColorBlendAttachmentState color_blend_attachment = {
        .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eA
    };

    vk::PipelineColorBlendStateCreateInfo color_blend_state = {
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment
    };

    std::array dynamic_states = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
    };

    vk::PipelineDynamicStateCreateInfo dynamic_state_create_info = {
        .dynamicStateCount = static_cast<RkUint32>(dynamic_states.size()),
        .pDynamicStates = dynamic_states.data()
    };

    vk::GraphicsPipelineCreateInfo pipeline_info = {
        .stageCount = static_cast<RkUint32>(shader_stages.size()),
        .pStages = shader_stages.data(),
        .pVertexInputState = &vertex_input_state,
        .pInputAssemblyState = &input_assembly_state,
        .pViewportState = &viewport_state,
        .pRasterizationState = &rasterization_state,
        .pMultisampleState = &multisample_state,
        .pDepthStencilState = &depth_stencil_state,
        .pColorBlendState = &color_blend_state,
        .pDynamicState = &dynamic_state_create_info,
        .layout = RenderPass::g_pipeline_layout,
        .renderPass = RenderPass::g_render_pass,
        .subpass = 0
    };

    m_pipeline = m_device->GetLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info).value;
}

Material::~Material() noexcept
{
    if (m_pipeline)
        m_device->GetLogicalDevice().destroy(m_pipeline);
}

RkVoid Material::Bind(vk::CommandBuffer const& in_command_buffer) const noexcept
{
    RkUint32 index = 0U;

    in_command_buffer.pushConstants(RenderPass::g_pipeline_layout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0U, 4U, &index);

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