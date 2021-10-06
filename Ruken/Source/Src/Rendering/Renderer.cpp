
#include "Rendering/Renderer.hpp"

#include "Windowing/WindowManager.hpp"

#include "Core/ServiceProvider.hpp"

#include "Rendering/Resources/Model.hpp"
#include "Rendering/Resources/Shader.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Static Variables

vk::RenderPass          Renderer::render_pass;
vk::DescriptorSetLayout Renderer::descriptor_set_layout;
vk::PipelineLayout      Renderer::pipeline_layout;
vk::Pipeline            Renderer::pipeline;

#pragma endregion

#pragma region Constructors

Renderer::Renderer(ServiceProvider& in_service_provider) noexcept: Service<Renderer> {in_service_provider}
{
    if (Logger* root_logger = m_service_provider.LocateService<Logger>())
        m_logger = root_logger->AddChild("Rendering");

    m_context = std::make_unique<RenderContext>(m_logger);
    m_device  = std::make_unique<RenderDevice> (m_logger, m_context.get());

    if (!m_context->GetInstance() || !m_device->GetLogicalDevice())
        return;

    Shader shader(m_device.get(), "Data/");

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

    render_pass = m_device->GetLogicalDevice().createRenderPass(render_pass_info).value;

    vk::DescriptorSetLayoutBinding bindings[2] = {
        {
            .binding = 0,
            .descriptorType = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1,
            .stageFlags = vk::ShaderStageFlagBits::eVertex
        },
        {
            .binding = 1,
            .descriptorType = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 1,
            .stageFlags = vk::ShaderStageFlagBits::eFragment
        }
    };

    vk::DescriptorSetLayoutCreateInfo set_layout_info = {
        .bindingCount = 2,
        .pBindings = bindings
    };

    descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(set_layout_info).value;

    vk::PipelineLayoutCreateInfo layout_info = {
        .setLayoutCount = 1,
        .pSetLayouts = &descriptor_set_layout
    };

    pipeline_layout = m_device->GetLogicalDevice().createPipelineLayout(layout_info).value;

    auto shader_stages = shader.GetShaderStages();

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
        vk::DynamicState::eViewport, vk::DynamicState::eScissor
    };

    vk::PipelineDynamicStateCreateInfo dynamic_state_create_info = {
        .dynamicStateCount = static_cast<RkUint32>(dynamic_states.size()),
        .pDynamicStates    = dynamic_states.data()
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
        .layout = pipeline_layout,
        .renderPass = render_pass,
        .subpass = 0
    };

    pipeline = m_device->GetLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipeline_info).value;

    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Renderer initialized."))
}

Renderer::~Renderer() noexcept
{
    if (!m_device->GetLogicalDevice())
        return;

    vk::Result const result = m_device->GetLogicalDevice().waitIdle();

    if (result != vk::Result::eSuccess)
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Error("Failed to shutdown renderer : " + vk::to_string(result)))

    m_device->GetLogicalDevice().destroy(pipeline);
    m_device->GetLogicalDevice().destroy(pipeline_layout);
    m_device->GetLogicalDevice().destroy(descriptor_set_layout);
    m_device->GetLogicalDevice().destroy(render_pass);

    m_device .reset();
    m_context.reset();

    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Renderer shutdown."))
}

#pragma endregion

#pragma region Methods

RenderContext* Renderer::GetContext() const noexcept
{
    return m_context.get();
}

RenderDevice* Renderer::GetDevice() const noexcept
{
    return m_device.get();
}

#pragma endregion