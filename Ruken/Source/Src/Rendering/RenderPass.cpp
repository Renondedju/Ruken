#include "Rendering/RenderPass.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"

USING_RUKEN_NAMESPACE

vk::RenderPass          RenderPass::g_render_pass;
vk::DescriptorSetLayout RenderPass::g_frame_descriptor_set_layout;
vk::DescriptorSetLayout RenderPass::g_camera_descriptor_set_layout;
vk::PipelineLayout      RenderPass::g_pipeline_layout;

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

    vk::DescriptorSetLayoutBinding second_bindings[3] = {
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
        }
    };

    vk::DescriptorSetLayoutBinding third_bindings[1] = {
        {
            .binding         = 0U,
            .descriptorType  = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1U,
            .stageFlags      = vk::ShaderStageFlagBits::eVertex
        }
    };

    vk::DescriptorSetLayoutCreateInfo set_layout_infos[2] = {
        {
            .bindingCount = 3U,
            .pBindings    = second_bindings
        },
        {
            .bindingCount = 1U,
            .pBindings    = third_bindings
        }
    };

    g_frame_descriptor_set_layout  = m_device->GetLogicalDevice().createDescriptorSetLayout(set_layout_infos[0]).value;
    g_camera_descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(set_layout_infos[1]).value;

    std::vector descriptor_set_layouts = {
        m_device->GetTextureDescriptorSetLayout(),
        g_frame_descriptor_set_layout,
        g_camera_descriptor_set_layout
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

    g_pipeline_layout = m_device->GetLogicalDevice().createPipelineLayout(layout_info).value;
}

RenderPass::~RenderPass() noexcept
{
    if (g_render_pass)
        m_device->GetLogicalDevice().destroy(g_render_pass);
    if (g_frame_descriptor_set_layout)
        m_device->GetLogicalDevice().destroy(g_frame_descriptor_set_layout);
    if (g_camera_descriptor_set_layout)
        m_device->GetLogicalDevice().destroy(g_camera_descriptor_set_layout);
    if (g_pipeline_layout)
        m_device->GetLogicalDevice().destroy(g_pipeline_layout);
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