#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

std::unique_ptr<Image> RenderGraph::g_color_target;
std::unique_ptr<Image> RenderGraph::g_depth_target;

RenderGraph::RenderGraph(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    vk::DescriptorSetLayoutBinding descriptor_set_layout_binding = {
        .binding         = 0U,
        .descriptorType  = vk::DescriptorType::eCombinedImageSampler,
        .descriptorCount = 1000U,
        .stageFlags      = vk::ShaderStageFlagBits::eFragment
    };

    vk::DescriptorBindingFlags descriptor_binding_flags = vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eVariableDescriptorCount;

    vk::DescriptorSetLayoutBindingFlagsCreateInfo descriptor_set_layout_binding_flags_create_info = {
        .bindingCount  = 1U,
        .pBindingFlags = &descriptor_binding_flags
    };

    vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
        .pNext        = &descriptor_set_layout_binding_flags_create_info,
        .bindingCount = 1U,
        .pBindings    = &descriptor_set_layout_binding
    };

    m_texture_descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(descriptor_set_layout_create_info).value;

    vk::DescriptorSetLayoutBinding frame_descriptor_set_layout_bindings[3] = {
        {
            .binding = 0U,
            .descriptorType = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
        },
        {
            .binding = 1U,
            .descriptorType = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags = vk::ShaderStageFlagBits::eVertex
        },
        {
            .binding = 2U,
            .descriptorType = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 1U,
            .stageFlags = vk::ShaderStageFlagBits::eFragment
        }
    };

    vk::DescriptorSetLayoutCreateInfo frame_descriptor_set_layout_create_info = {
        .bindingCount = 3U,
        .pBindings    = frame_descriptor_set_layout_bindings
    };

    m_frame_descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(frame_descriptor_set_layout_create_info).value;

    vk::DescriptorSetLayoutBinding camera_descriptor_set_layout_bindings[1] = {
        {
            .binding = 0U,
            .descriptorType = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1U,
            .stageFlags = vk::ShaderStageFlagBits::eVertex
        }
    };

    vk::DescriptorSetLayoutCreateInfo camera_descriptor_set_layout_create_info = {
        .bindingCount = 1U,
        .pBindings    = camera_descriptor_set_layout_bindings
    };

    m_camera_descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(camera_descriptor_set_layout_create_info).value;

    std::vector descriptor_set_layouts = {
        m_texture_descriptor_set_layout,
        m_frame_descriptor_set_layout,
        m_camera_descriptor_set_layout,
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

    m_default_pipeline_layout = m_device->GetLogicalDevice().createPipelineLayout(layout_info).value;

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_frames.emplace_back(std::make_unique<RenderFrame>(m_logger, m_device, this));
    }

    vk::ImageCreateInfo image_create_info = {
        .imageType   = vk::ImageType::e2D,
        .format      = vk::Format::eR8G8B8A8Unorm,
        .extent      = {
            .width  = 1920U,
            .height = 1080U,
            .depth  = 1U
        },
        .mipLevels   = 1,
        .arrayLayers = 1,
        .tiling      = vk::ImageTiling::eOptimal,
        .usage       = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc
    };

    g_color_target = std::make_unique<Image>(m_device, image_create_info);

    image_create_info.format = vk::Format::eD32Sfloat;
    image_create_info.usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    g_depth_target = std::make_unique<Image>(m_device, image_create_info);
}

RenderGraph::~RenderGraph() noexcept
{
    g_color_target.reset();
    g_depth_target.reset();

    if (m_texture_descriptor_set_layout)
        m_device->GetLogicalDevice().destroy(m_texture_descriptor_set_layout);
    if (m_frame_descriptor_set_layout)
        m_device->GetLogicalDevice().destroy(m_frame_descriptor_set_layout);
    if (m_camera_descriptor_set_layout)
        m_device->GetLogicalDevice().destroy(m_camera_descriptor_set_layout);
    if (m_default_pipeline_layout)
        m_device->GetLogicalDevice().destroy(m_default_pipeline_layout);
}

RkVoid RenderGraph::Build() noexcept
{
    for (auto const& pass : m_passes)
    {
        pass.second->Build();
    }
}

RkVoid RenderGraph::Execute(vk::Semaphore const& in_wait_semaphore, vk::Semaphore const& in_signal_semaphore) noexcept
{
    auto& frame = GetCurrentFrame();

    for (auto const& pass : m_passes)
    {
        pass.second->Execute();
    }

    // TODO : Final pass to swapchain.
    auto command_buffer = frame.GetGraphicsCommandPool().Request();

    vk::CommandBufferBeginInfo command_buffer_begin_info = {

    };

    if (command_buffer.begin(command_buffer_begin_info) == vk::Result::eSuccess)
    {
        vk::ImageMemoryBarrier render_target_barrier = {
            .srcAccessMask    = vk::AccessFlagBits::eColorAttachmentWrite,
            .dstAccessMask    = vk::AccessFlagBits::eTransferRead,
            .oldLayout        = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout        = vk::ImageLayout::eTransferSrcOptimal,
            .image            = g_color_target->GetHandle(),
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            render_target_barrier);

        vk::ImageMemoryBarrier swapchain_barrier = {
            .srcAccessMask    = vk::AccessFlagBits::eNoneKHR,
            .dstAccessMask    = vk::AccessFlagBits::eTransferWrite,
            .oldLayout        = vk::ImageLayout::eUndefined,
            .newLayout        = vk::ImageLayout::eTransferDstOptimal,
            .image            = m_final_target->GetHandle(),
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            swapchain_barrier);

        vk::ImageBlit image_blit = {
            .srcSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            },
            .dstSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            }
        };

        image_blit.srcOffsets[1].x = g_color_target->GetExtent().width;
        image_blit.srcOffsets[1].y = g_color_target->GetExtent().height;
        image_blit.srcOffsets[1].z = 1U;
        image_blit.dstOffsets[1].x = m_final_target->GetExtent().width;
        image_blit.dstOffsets[1].y = m_final_target->GetExtent().height;
        image_blit.dstOffsets[1].z = 1U;

        command_buffer.blitImage(
            g_color_target->GetHandle(),
            vk::ImageLayout::eTransferSrcOptimal,
            m_final_target->GetHandle(),
            vk::ImageLayout::eTransferDstOptimal,
            1U,
            &image_blit,
            vk::Filter::eLinear);

        render_target_barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
        render_target_barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        render_target_barrier.oldLayout     = vk::ImageLayout::eTransferSrcOptimal;
        render_target_barrier.newLayout     = vk::ImageLayout::eColorAttachmentOptimal;
        
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            render_target_barrier);

        swapchain_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        swapchain_barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
        swapchain_barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
        swapchain_barrier.newLayout     = vk::ImageLayout::ePresentSrcKHR;
        
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            swapchain_barrier);

        if (command_buffer.end() == vk::Result::eSuccess)
        {
            std::vector<vk::SemaphoreSubmitInfoKHR> wait_semaphores_submit_infos = {
                {
                    .semaphore = in_wait_semaphore
                },
                {
                    .semaphore = frame.GetTimelineSemaphore().GetHandle(),
                    .value     = frame.GetTimelineSemaphore().GetValue ()
                }
            };

            std::vector<vk::CommandBufferSubmitInfoKHR> command_buffer_submit_infos = {
                {
                    .commandBuffer = command_buffer
                }
            };

            std::vector<vk::SemaphoreSubmitInfoKHR> signal_semaphores_submit_infos = {
                {
                    .semaphore = in_signal_semaphore
                },
                {
                    .semaphore = frame.GetTimelineSemaphore().GetHandle(),
                    .value     = frame.GetTimelineSemaphore().NextValue()
                }
            };

            vk::SubmitInfo2KHR submit_info = {
                .waitSemaphoreInfoCount   = static_cast<RkUint32>(wait_semaphores_submit_infos.size()),
                .pWaitSemaphoreInfos      = wait_semaphores_submit_infos.data(),
                .commandBufferInfoCount   = static_cast<RkUint32>(command_buffer_submit_infos.size()),
                .pCommandBufferInfos      = command_buffer_submit_infos.data(),
                .signalSemaphoreInfoCount = static_cast<RkUint32>(signal_semaphores_submit_infos.size()),
                .pSignalSemaphoreInfos    = signal_semaphores_submit_infos.data()
            };

            m_device->GetGraphicsQueue().Submit(submit_info);
        }
    }

    frame.GetGraphicsCommandPool().Release(command_buffer);
}

RenderPass& RenderGraph::AddPass(std::string const& in_name) noexcept
{
    if (!m_passes.contains(in_name))
        m_passes[in_name] = std::make_unique<RenderPass>(m_logger, m_device, this);

    return *m_passes.at(in_name);
}

RenderPass* RenderGraph::FindPass(std::string const& in_name) const noexcept
{
    if (!m_passes.contains(in_name))
        return nullptr;

    return m_passes.at(in_name).get();
}

RkVoid RenderGraph::SetFinalTarget(Image const& in_target) noexcept
{
    m_final_target = &in_target;
}

vk::DescriptorSetLayout const& RenderGraph::GetTextureDescriptorSetLayout() const noexcept
{
    return m_texture_descriptor_set_layout;
}

vk::DescriptorSetLayout const& RenderGraph::GetFrameDescriptorSetLayout() const noexcept
{
    return m_frame_descriptor_set_layout;
}

vk::DescriptorSetLayout const& RenderGraph::GetCameraDescriptorSetLayout() const noexcept
{
    return m_camera_descriptor_set_layout;    
}

vk::PipelineLayout const& RenderGraph::GetDefaultPipelineLayout() const noexcept
{
    return m_default_pipeline_layout;
}

RenderFrame& RenderGraph::BeginFrame() noexcept
{
    RenderFrame& current_frame = *m_frames[m_frame_index];

    current_frame.Reset();

    return current_frame;
}

RenderFrame& RenderGraph::GetCurrentFrame() noexcept
{
    return *m_frames[m_frame_index];
}

RkVoid RenderGraph::EndFrame() noexcept
{
    m_frame_index = (m_frame_index + 1U) % static_cast<RkUint32>(m_frames.size());
}

RenderImageResource& RenderGraph::GetImageResource(std::string const& in_name) noexcept
{
    if (!m_resources.contains(in_name))
    {
        m_resources.emplace(in_name, std::make_unique<RenderImageResource>());
    }

    return *static_cast<RenderImageResource*>(m_resources[in_name].get());
}

RenderBufferResource& RenderGraph::GetBufferResource(std::string const& in_name) noexcept
{
    if (!m_resources.contains(in_name))
    {
        m_resources.emplace(in_name, std::make_unique<RenderBufferResource>());
    }

    return *static_cast<RenderBufferResource*>(m_resources[in_name].get());
}