#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderFrame.hpp"

USING_RUKEN_NAMESPACE

RenderGraph::RenderGraph(Logger* in_logger, Renderer* in_renderer) noexcept:
    m_logger   {in_logger},
    m_renderer {in_renderer}
{
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

    m_frame_descriptor_set_layout  = m_renderer->GetDevice()->GetLogicalDevice().createDescriptorSetLayout(frame_descriptor_set_layout_create_info).value;

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

    m_camera_descriptor_set_layout = m_renderer->GetDevice()->GetLogicalDevice().createDescriptorSetLayout(camera_descriptor_set_layout_create_info).value;
}

RenderGraph::~RenderGraph() noexcept
{
    if (m_frame_descriptor_set_layout)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_frame_descriptor_set_layout);
    if (m_camera_descriptor_set_layout)
        m_renderer->GetDevice()->GetLogicalDevice().destroy(m_camera_descriptor_set_layout);
}

RkVoid RenderGraph::Bake() noexcept
{
    
}

RkVoid RenderGraph::Execute(RenderFrame& in_frame) noexcept
{
    for (auto it = m_render_passes.begin(); it != m_render_passes.end(); ++it)
    {
        it->second->Execute(in_frame);
    }

    // TODO : Final pass to swapchain.
}

RenderPass& RenderGraph::FindOrAddRenderPass(std::string const& in_name) noexcept
{
    if (!m_render_passes.contains(in_name))
    {
        m_render_passes[in_name] = std::make_unique<RenderPass>(m_logger, m_renderer);
    }

    return *m_render_passes.at(in_name);
}

RenderTarget& RenderGraph::FindOrAddRenderTarget(std::string const& in_name, AttachmentInfo const& in_attachment_info) noexcept
{
    if (!m_render_targets.contains(in_name))
    {
        m_render_targets[in_name] = std::make_unique<RenderTarget>(m_renderer->GetDevice(), in_attachment_info);
    }

    return *m_render_targets.at(in_name);
}

RenderTarget& RenderGraph::FindRenderTarget(std::string const& in_name) noexcept
{
    return *m_render_targets.at(in_name); 
}

vk::DescriptorSetLayout const& RenderGraph::GetFrameDescriptorSetLayout() const noexcept
{
    return m_frame_descriptor_set_layout;
}

vk::DescriptorSetLayout const& RenderGraph::GetCameraDescriptorSetLayout() const noexcept
{
    return m_camera_descriptor_set_layout;    
}