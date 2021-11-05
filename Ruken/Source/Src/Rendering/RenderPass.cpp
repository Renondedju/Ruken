#include "Rendering/RenderPass.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

RenderPass::RenderPass(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph) noexcept:
    m_logger {in_logger},
    m_device {in_device},
    m_graph  {in_graph}
{
    
}

RenderPass::~RenderPass() noexcept
{
    if (m_handle)
        m_device->GetLogicalDevice().destroy(m_handle);
    if (m_framebuffer)
        m_device->GetLogicalDevice().destroy(m_framebuffer);
}

#pragma endregion

#pragma region Methods

RkVoid RenderPass::Build() noexcept
{
    if (m_handle)
        return;

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

    m_handle = m_device->GetLogicalDevice().createRenderPass(render_pass_info).value;

    std::vector formats = {
        vk::Format::eR8G8B8A8Unorm,
        vk::Format::eD32Sfloat
    };

    std::vector<vk::FramebufferAttachmentImageInfo> framebuffer_attachment_image_infos = {
        vk::FramebufferAttachmentImageInfo {
            .usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
            .width = 1920,
            .height = 1080,
            .layerCount = 1,
            .viewFormatCount = static_cast<RkUint32>(formats.size()),
            .pViewFormats = formats.data()
        },
        vk::FramebufferAttachmentImageInfo {
            .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment,
            .width = 1920,
            .height = 1080,
            .layerCount = 1,
            .viewFormatCount = static_cast<RkUint32>(formats.size()),
            .pViewFormats = formats.data()
        }
    };

    vk::FramebufferAttachmentsCreateInfo framebuffer_attachments_create_info = {
        .attachmentImageInfoCount = static_cast<RkUint32>(framebuffer_attachment_image_infos.size()),
        .pAttachmentImageInfos    = framebuffer_attachment_image_infos.data()
    };

    vk::FramebufferCreateInfo framebuffer_create_info = {
        .pNext           = &framebuffer_attachments_create_info,
        .flags           = vk::FramebufferCreateFlagBits::eImageless,
        .renderPass      = m_handle,
        .attachmentCount = static_cast<RkUint32>(framebuffer_attachment_image_infos.size()),
        .width           = 1920,
        .height          = 1080,
        .layers          = 1,
    };

    m_framebuffer = m_device->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value;
}

RkVoid RenderPass::Execute() const noexcept
{
    auto& frame = m_graph->GetCurrentFrame();

    auto command_buffer = frame.GetGraphicsCommandPool().Request();

    vk::CommandBufferBeginInfo command_buffer_begin_info = {};

    if (command_buffer.begin(command_buffer_begin_info) == vk::Result::eSuccess)
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

        std::vector image_views = {
            RenderGraph::g_color_target->GetView(),
            RenderGraph::g_depth_target->GetView()
        };

        vk::RenderPassAttachmentBeginInfo render_pass_attachment_begin_info = {
            .attachmentCount = static_cast<RkUint32>(image_views.size()),
            .pAttachments    = image_views.data()
        };

        vk::ClearValue clear_color = {
            .color = {
                std::array{0.0F, 0.0F, 0.0F, 1.0F}
            }
        };

        vk::ClearValue clear_stencil = {
            .depthStencil = {1, 0}
        };

        std::vector clear_values = {
            clear_color,
            clear_stencil
        };

        vk::RenderPassBeginInfo render_pass_begin_info = {
            .pNext       = &render_pass_attachment_begin_info,
            .renderPass  = m_handle,
            .framebuffer = m_framebuffer,
            .renderArea = {
                .extent = {
                    .width  = 1920U,
                    .height = 1080U
                }
            },
            .clearValueCount = static_cast<RkUint32>(clear_values.size()),
            .pClearValues    = clear_values.data()
        };

        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        std::vector descriptor_sets = {
            frame.GetTextureDescriptorSet(),
            frame.GetFrameDescriptorSet(),
            frame.GetCameraDescriptorSet()
        };

        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_graph->GetDefaultPipelineLayout(), 0U, descriptor_sets, nullptr);

        if (m_callback)
            m_callback(command_buffer);

        command_buffer.endRenderPass();

        if (command_buffer.end() == vk::Result::eSuccess)
        {
            vk::CommandBufferSubmitInfoKHR command_buffer_submit_info = {
                .commandBuffer = command_buffer
            };

            vk::SemaphoreSubmitInfoKHR timeline_semaphore_submit_info = {
                .semaphore = frame.GetTimelineSemaphore().GetHandle(),
                .value     = frame.GetTimelineSemaphore().NextValue()
            };

            vk::SubmitInfo2KHR submit_info = {
                .commandBufferInfoCount = 1U,
                .pCommandBufferInfos = &command_buffer_submit_info,
                .signalSemaphoreInfoCount = 1U,
                .pSignalSemaphoreInfos = &timeline_semaphore_submit_info
            };

            m_device->GetGraphicsQueue().Submit(submit_info);
        }
    }

    frame.GetGraphicsCommandPool().Release(command_buffer);
}

RkVoid RenderPass::AddColorInput(std::string const& in_name)
{
    auto& res = m_graph->GetImageResource(in_name);

    m_color_inputs.push_back(&res);
}

RkVoid RenderPass::AddColorOutput(std::string const& in_name, ImageInfo const& in_image_info)
{
    (void)in_image_info;

    auto& res = m_graph->GetImageResource(in_name);

    m_color_outputs.push_back(&res);
}

RkVoid RenderPass::SetDepthStencilInput(std::string const& in_name)
{
    auto& res = m_graph->GetImageResource(in_name);

    m_depth_stencil_input = &res;
}

RkVoid RenderPass::SetDepthStencilOutput(std::string const& in_name, ImageInfo const& in_image_info)
{
    (void)in_image_info;

    auto& res = m_graph->GetImageResource(in_name);

    m_depth_stencil_output = &res;
}

RkVoid RenderPass::SetCallback(std::function<RkVoid(vk::CommandBuffer const&)>&& in_callback) noexcept
{
    m_callback = std::move(in_callback);
}

vk::RenderPass const& RenderPass::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion