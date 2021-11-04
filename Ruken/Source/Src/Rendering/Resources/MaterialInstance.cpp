#include "Rendering/Resources/MaterialInstance.hpp"
#include "Rendering/Resources/Model.hpp"

#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

MaterialInstance::MaterialInstance(Renderer* in_renderer, Material* in_material) noexcept:
    m_renderer {in_renderer},
    m_material {in_material}
{
    vk::GraphicsPipelineCreateInfo pipeline_info = {
        .stageCount          = static_cast<RkUint32>(in_material->GetShaderStages().size()),
        .pStages             = in_material->GetShaderStages().data(),
        .pVertexInputState   = &in_material->GetVertexInputState(),
        .pInputAssemblyState = &in_material->GetInputAssemblyState(),
        .pViewportState      = &in_material->GetViewportState(),
        .pRasterizationState = &in_material->GetRasterizationState(),
        .pMultisampleState   = &in_material->GetMultisampleState(),
        .pDepthStencilState  = &in_material->GetDepthStencilState(),
        .pColorBlendState    = &in_material->GetColorBlendState(),
        .pDynamicState       = &in_material->GetDynamicState(),
        .layout              = in_material->GetPipelineLayout(),
        .renderPass          = m_renderer->GetGraph()->FindOrAddRenderPass("Forward").GetHandle(),
        .basePipelineHandle  = in_material->GetPipeline()
    };

    m_pipeline = m_renderer->GetDevice()->GetLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info).value;
}

MaterialInstance::~MaterialInstance() noexcept
{
    if (m_pipeline)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_pipeline);
}

RkVoid MaterialInstance::Bind(vk::CommandBuffer const& in_command_buffer) const noexcept
{
    in_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline);
}

RkVoid MaterialInstance::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid MaterialInstance::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid MaterialInstance::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}