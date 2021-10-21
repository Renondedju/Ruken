#include "Rendering/RenderPass.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"

#include "Rendering/Resources/Model.hpp"
#include "Rendering/Resources/Shader.hpp"

USING_RUKEN_NAMESPACE

vk::RenderPass          RenderPass::g_render_pass;
std::array<vk::DescriptorSetLayout, 2> RenderPass::g_descriptor_set_layouts;
vk::PipelineLayout      RenderPass::g_pipeline_layout;
vk::Pipeline            RenderPass::g_pipeline;

#pragma region Constructors

RenderPass::RenderPass(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph, std::string const& in_name) noexcept:
    m_logger {in_logger},
    m_device {in_device},
    m_graph  {in_graph},
    m_name   {in_name}
{
    vk::AttachmentReference color_attachment_reference = {
        .attachment = 0,
        .layout = vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::AttachmentReference depth_attachment_reference = {
        .attachment = 1,
        .layout = vk::ImageLayout::eDepthStencilAttachmentOptimal
    };

    vk::SubpassDescription subpass = {
        .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_reference,
        .pDepthStencilAttachment = &depth_attachment_reference
    };

    std::array<vk::AttachmentDescription, 2> attachments = {
        {
            {
                .format = vk::Format::eR8G8B8A8Unorm,
                .samples = vk::SampleCountFlagBits::e1,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                .initialLayout = vk::ImageLayout::eUndefined,
                .finalLayout = vk::ImageLayout::eColorAttachmentOptimal
            },
            {
                .format = vk::Format::eD24UnormS8Uint,
                .samples = vk::SampleCountFlagBits::e1,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eDontCare,
                .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                .initialLayout = vk::ImageLayout::eUndefined,
                .finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal
            }
        }
    };

    vk::SubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .srcAccessMask = vk::AccessFlagBits::eNoneKHR,
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
    };

    vk::RenderPassCreateInfo render_pass_info = {
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    g_render_pass = m_device->GetLogicalDevice().createRenderPass(render_pass_info).value;

    vk::DescriptorSetLayoutBinding first_bindings[4] = {
        {
            .binding         = 0U,
            .descriptorType  = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
        },
        {
            .binding         = 1U,
            .descriptorType  = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eVertex
        },
        {
            .binding         = 2U,
            .descriptorType  = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eFragment
        },
        {
            .binding         = 3U,
            .descriptorType  = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eFragment
        }
    };

    vk::DescriptorSetLayoutBinding second_bindings[1] = {
        {
            .binding         = 0U,
            .descriptorType  = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eVertex
        }
    };

    vk::DescriptorBindingFlags binding_flags[4];

    binding_flags[3] = vk::DescriptorBindingFlagBits::eUpdateAfterBind
                     | vk::DescriptorBindingFlagBits::ePartiallyBound
                     | vk::DescriptorBindingFlagBits::eVariableDescriptorCount;

    vk::DescriptorSetLayoutBindingFlagsCreateInfo binding_flags_create_info = {
        .bindingCount  = 4U,
        .pBindingFlags = binding_flags
    };

    vk::DescriptorSetLayoutCreateInfo set_layout_infos[2] = {
        {
            .pNext        = &binding_flags_create_info,
            .flags        = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
            .bindingCount = 4U,
            .pBindings    = first_bindings
        },
        {
            .bindingCount = 1U,
            .pBindings    = second_bindings
        }
    };

    vk::PushConstantRange push_constant_range = {
        .stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
        .size       = sizeof(RkUint32)
    };

    g_descriptor_set_layouts[0] = m_device->GetLogicalDevice().createDescriptorSetLayout(set_layout_infos[0]).value;
    g_descriptor_set_layouts[1] = m_device->GetLogicalDevice().createDescriptorSetLayout(set_layout_infos[1]).value;

    vk::PipelineLayoutCreateInfo layout_info = {
        .setLayoutCount         = 2U,
        .pSetLayouts            = g_descriptor_set_layouts.data(),
        .pushConstantRangeCount = 1U,
        .pPushConstantRanges    = &push_constant_range
    };

    g_pipeline_layout = m_device->GetLogicalDevice().createPipelineLayout(layout_info).value;

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
        .layout = g_pipeline_layout,
        .renderPass = g_render_pass,
        .subpass = 0
    };

    g_pipeline = m_device->GetLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info).value;
}

RenderPass::~RenderPass() noexcept
{
    if (g_render_pass)
        m_device->GetLogicalDevice().destroy(g_render_pass);
    if (g_pipeline)
        m_device->GetLogicalDevice().destroy(g_pipeline);
    if (g_pipeline_layout)
        m_device->GetLogicalDevice().destroy(g_pipeline_layout);

    for (auto const& descriptor_set_layout : g_descriptor_set_layouts)
        m_device->GetLogicalDevice().destroy(descriptor_set_layout);
}

#pragma endregion

#pragma region Methods

RkVoid RenderPass::Execute(vk::CommandBuffer const& in_command_buffer, RenderFrame const& in_frame) const noexcept
{
    vk::ClearValue clear_color = {
        .color = {
            std::array{0.0F, 0.0F, 0.0F, 1.0F}
        }
    };

    vk::ClearValue clear_stencil = {
        .depthStencil = {1, 0}
    };

    std::array clear_values = {
        clear_color,
        clear_stencil
    };

    vk::RenderPassBeginInfo render_pass_begin_info = {
        .renderPass = g_render_pass,
        .framebuffer = in_frame.GetFramebuffer(),
        .renderArea = {
            .extent = {
                .width  = in_frame.GetColorTarget().GetExtent().width,
                .height = in_frame.GetColorTarget().GetExtent().height
            }
        },
        .clearValueCount = static_cast<RkUint32>(clear_values.size()),
        .pClearValues = clear_values.data()
    };

    in_command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

    in_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, g_pipeline);

    if (m_callback)
        m_callback(in_command_buffer);

    in_command_buffer.endRenderPass();
}

RkVoid RenderPass::SetCallback(std::function<RkVoid(vk::CommandBuffer const&)>&& in_callback) noexcept
{
    m_callback = std::move(in_callback);
}

#pragma region Inputs



#pragma endregion

#pragma region Outputs

RkVoid RenderPass::AddColorOutput(std::string const& in_name) noexcept
{
    (void)in_name;
}

#pragma endregion

std::string const& RenderPass::GetName() const noexcept
{
    return m_name;
}

#pragma endregion