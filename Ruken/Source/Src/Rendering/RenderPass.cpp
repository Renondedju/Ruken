#include "Rendering/RenderPass.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

RenderPass::RenderPass(Logger* in_logger, Renderer* in_renderer) noexcept:
    m_logger   {in_logger},
    m_renderer {in_renderer}
{
    vk::AttachmentReference color_attachment_reference = {
        .attachment = 0,
        .layout     = vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::AttachmentReference depth_attachment_reference = {
        .attachment = 1,
        .layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal
    };

    vk::SubpassDescription subpass = {
        .pipelineBindPoint       = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount    = 1,
        .pColorAttachments       = &color_attachment_reference,
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
                .format = vk::Format::eD32Sfloat,
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

    m_handle = m_renderer->GetDevice()->GetLogicalDevice().createRenderPass(render_pass_info).value;

    

    std::vector descriptor_set_layouts = {
        m_renderer->GetTextureStreamer()->GetDescriptorSetLayout(),
        m_renderer->GetGraph()->GetFrameDescriptorSetLayout(),
        m_renderer->GetGraph()->GetCameraDescriptorSetLayout(),
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

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        vk::FramebufferCreateInfo framebuffer_create_info = {
            .renderPass = m_handle,
            .width      = 1920U,
            .height     = 1080U,
            .layers     = 1U
        };

        // m_framebuffers.emplace_back(m_renderer->GetDevice()->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value);
    }
}

RenderPass::~RenderPass() noexcept
{
    if (m_handle)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_handle);
    if (m_pipeline_layout)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_pipeline_layout);
}

#pragma endregion

#pragma region Methods

RkVoid RenderPass::Execute(RenderFrame& in_frame) const noexcept
{
    auto command_buffer = in_frame.GetGraphicsCommandPool().Request();

    vk::CommandBufferBeginInfo command_buffer_begin_info = {};

    if (command_buffer.begin(command_buffer_begin_info) != vk::Result::eSuccess)
    {
        vk::Viewport viewport = {
            .width = 1920.0F,
            .height = 1080.0F,
            .minDepth = 0.0F,
            .maxDepth = 1.0F
        };

        vk::Rect2D scissor = {
            .extent = {
                1920U,
                1080U
            }
        };

        command_buffer.setViewport(0, viewport);
        command_buffer.setScissor(0, scissor);

        vk::RenderPassBeginInfo render_pass_begin_info = {
            .renderPass = m_handle,
            .framebuffer = m_framebuffers[in_frame.GetIndex()],
            .renderArea = {
                .extent = {
                    .width  = 1920U,
                    .height = 1080U
                }
            }
        };

        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        std::vector descriptor_sets = {
            m_renderer->GetTextureStreamer()->GetDescriptorSet(),
            in_frame.GetFrameDescriptorSet(),
            in_frame.GetCameraDescriptorSet()
        };

        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline_layout, 0U, descriptor_sets, nullptr);

        if (m_callback)
            m_callback(command_buffer, in_frame);

        command_buffer.endRenderPass();

        if (command_buffer.end() != vk::Result::eSuccess)
        {
            vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
                .commandBuffer = command_buffer
            };

            vk::SemaphoreSubmitInfoKHR timeline_semaphore_submit_info = {
                .semaphore = in_frame.GetTimelineSemaphore().GetHandle(),
                .value = in_frame.GetTimelineSemaphore().NextValue()
            };

            vk::SubmitInfo2KHR submit_info = {
                .commandBufferInfoCount = 1U,
                .pCommandBufferInfos = &command_buffer_submit_info,
                .signalSemaphoreInfoCount = 1U,
                .pSignalSemaphoreInfos = &timeline_semaphore_submit_info
            };

            m_renderer->GetDevice()->GetGraphicsQueue().Submit(submit_info);
        }
    }

    in_frame.GetGraphicsCommandPool().Release(command_buffer);
}

RkVoid RenderPass::AddColorOutput(std::string const& in_name)
{
    (void)in_name;
}

RkVoid RenderPass::SetCallback(std::function<RkVoid(vk::CommandBuffer const&, RenderFrame const&)>&& in_callback) noexcept
{
    m_callback = std::move(in_callback);
}

vk::RenderPass const& RenderPass::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion