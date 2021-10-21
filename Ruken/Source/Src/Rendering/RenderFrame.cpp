#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderPass.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger                   {in_logger},
    m_device                   {in_device},
    m_timeline_semaphore       {in_device},
    m_semaphore_pool           {in_device},
    m_graphics_command_pool    {in_device, in_device->GetGraphicsFamilyIndex()},
    m_compute_command_pool     {in_device, in_device->GetComputeFamilyIndex ()},
    m_transfer_command_pool    {in_device, in_device->GetTransferFamilyIndex()},
    m_draw_storage_buffer      {in_device, 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_transform_storage_buffer {in_device, 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_material_storage_buffer  {in_device, 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_camera_uniform_buffer    {in_device, sizeof(CameraData), vk::BufferUsageFlagBits::eUniformBuffer}
{
    m_color_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eR8G8B8A8Unorm,  vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc);
    m_depth_target = std::make_unique<RenderTarget>(m_device, 1920, 1080, vk::Format::eD24UnormS8Uint, vk::ImageUsageFlagBits::eDepthStencilAttachment);

    std::array attachments = {
        m_color_target->GetImageView(),
        m_depth_target->GetImageView()
    };

    vk::FramebufferCreateInfo framebuffer_create_info = {
        .renderPass      = RenderPass::g_render_pass,
        .attachmentCount = static_cast<RkUint32>(attachments.size()),
        .pAttachments    = attachments.data(),
        .width           = 1920,
        .height          = 1080,
        .layers          = 1
    };

    m_framebuffer = m_device->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value;

    vk::DescriptorPoolSize pool_sizes[4] = {
        {
            .type            = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 10U
        },
        {
            .type            = vk::DescriptorType::eSampledImage,
            .descriptorCount = 10U
        },
        {
            .type            = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 10U
        },
        {
            .type            = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 10U
        }
    };

    vk::DescriptorPoolCreateInfo descriptor_pool_info = {
        .flags         = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
        .maxSets       = 100U,
        .poolSizeCount = 4U,
        .pPoolSizes    = pool_sizes
    };

    m_global_descriptor_pool = m_device->GetLogicalDevice().createDescriptorPool(descriptor_pool_info).value;

    RkUint32 count = 1U;

    vk::DescriptorSetVariableDescriptorCountAllocateInfo set_variable_descriptor_count_allocate_info = {
        .descriptorSetCount = 1U,
        .pDescriptorCounts  = &count
    };

    vk::DescriptorSetAllocateInfo global_descriptor_set_allocate_info = {
        .pNext              = &set_variable_descriptor_count_allocate_info,
        .descriptorPool     = m_global_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &RenderPass::g_descriptor_set_layouts[0]
    };

    vk::DescriptorSetAllocateInfo camera_descriptor_set_allocate_info = {
        .descriptorPool     = m_global_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &RenderPass::g_descriptor_set_layouts[1]
    };

    m_global_descriptor_set = m_device->GetLogicalDevice().allocateDescriptorSets(global_descriptor_set_allocate_info).value.front();
    m_camera_descriptor_set = m_device->GetLogicalDevice().allocateDescriptorSets(camera_descriptor_set_allocate_info).value.front();

    vk::DescriptorBufferInfo camera_descriptor_buffer_info = {
        .buffer = m_camera_uniform_buffer.GetHandle(),
        .offset = 0ULL,
        .range  = sizeof(CameraData)
    };

    vk::WriteDescriptorSet write_descriptor = {
        .dstSet = m_camera_descriptor_set,
        .dstBinding = 0,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = vk::DescriptorType::eUniformBuffer,
        .pBufferInfo = &camera_descriptor_buffer_info
    };

    m_device->GetLogicalDevice().updateDescriptorSets(write_descriptor, VK_NULL_HANDLE);

    CameraData ubo = {
        .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 10.0f)
    };

    ubo.proj[1][1] *= -1;

    memcpy(m_camera_uniform_buffer.Map(), &ubo, sizeof(CameraData));

    m_camera_uniform_buffer.UnMap();
}

RenderFrame::~RenderFrame() noexcept
{
    m_color_target.reset();
    m_depth_target.reset();

    m_device->GetLogicalDevice().destroy(m_framebuffer);
    m_device->GetLogicalDevice().destroy(m_global_descriptor_pool);
}

RkVoid RenderFrame::Reset() noexcept
{
    vk::SemaphoreWaitInfo semaphore_wait_info = {
        .semaphoreCount = 1U,
        .pSemaphores    = &m_timeline_semaphore.GetHandle(),
        .pValues        = &m_timeline_semaphore.GetValue ()
    };

    if (m_device->GetLogicalDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX) != vk::Result::eSuccess)
    {

    }

    m_semaphore_pool       .Reset();
    m_graphics_command_pool.Reset();
    m_compute_command_pool .Reset();
    m_transfer_command_pool.Reset();
}

RkVoid RenderFrame::Bind(vk::CommandBuffer const& in_command_buffer) noexcept
{
    std::vector descriptor_sets = {
        m_global_descriptor_set, m_camera_descriptor_set
    };

    in_command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, RenderPass::g_pipeline_layout, 0U, descriptor_sets, nullptr);
}

RenderTarget const& RenderFrame::GetColorTarget() const noexcept
{
    return *m_color_target;
}

RenderTarget const& RenderFrame::GetDepthTarget() const noexcept
{
    return *m_depth_target;
}

vk::Framebuffer const& RenderFrame::GetFramebuffer() const noexcept
{
    return m_framebuffer;
}

vk::DescriptorSet const& RenderFrame::GetGlobalDescriptorSet() const noexcept
{
    return m_global_descriptor_set;
}

TimelineSemaphore& RenderFrame::GetTimelineSemaphore() noexcept
{
    return m_timeline_semaphore;
}

SemaphorePool& RenderFrame::GetSemaphorePool() noexcept
{
    return m_semaphore_pool;
}

CommandPool& RenderFrame::GetGraphicsCommandPool() noexcept
{
    return m_graphics_command_pool;
}

CommandPool& RenderFrame::GetComputeCommandPool() noexcept
{
    return m_compute_command_pool;
}

CommandPool& RenderFrame::GetTransferCommandPool() noexcept
{
    return m_transfer_command_pool;
}

Buffer& RenderFrame::GetDrawStorageBuffer() noexcept
{
    return m_draw_storage_buffer;
}

Buffer& RenderFrame::GetMaterialStorageBuffer() noexcept
{
    return m_material_storage_buffer;
}

Buffer& RenderFrame::GetTransformStorageBuffer() noexcept
{
    return m_transform_storage_buffer;
}
}
