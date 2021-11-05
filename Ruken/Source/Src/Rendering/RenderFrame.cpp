#include "Rendering/RenderFrame.hpp"
#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(Logger* in_logger, Renderer* in_renderer, RkUint32 const in_index) noexcept:
    m_logger                   {in_logger},
    m_renderer                 {in_renderer},
    m_index                    {in_index},
    m_timeline_semaphore       {in_renderer->GetDevice()},
    m_semaphore_pool           {in_renderer->GetDevice()},
    m_graphics_command_pool    {in_renderer->GetDevice(), in_renderer->GetDevice()->GetGraphicsFamilyIndex()},
    m_compute_command_pool     {in_renderer->GetDevice(), in_renderer->GetDevice()->GetComputeFamilyIndex ()},
    m_transfer_command_pool    {in_renderer->GetDevice(), in_renderer->GetDevice()->GetTransferFamilyIndex()},
    m_draw_storage_buffer      {in_renderer->GetDevice(), 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_transform_storage_buffer {in_renderer->GetDevice(), 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_material_storage_buffer  {in_renderer->GetDevice(), 0ULL, vk::BufferUsageFlagBits::eStorageBuffer},
    m_camera_uniform_buffer    {in_renderer->GetDevice(), sizeof(CameraData), vk::BufferUsageFlagBits::eUniformBuffer}
{
    vk::DescriptorPoolSize pool_sizes[2] = {
        {
            .type            = vk::DescriptorType::eStorageBuffer,
            .descriptorCount = 3U
        },
        {
            .type            = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1U
        }
    };

    vk::DescriptorPoolCreateInfo descriptor_pool_info = {
        .maxSets       = 2U,
        .poolSizeCount = 2U,
        .pPoolSizes    = pool_sizes
    };

    m_descriptor_pool = m_renderer->GetDevice()->GetLogicalDevice().createDescriptorPool(descriptor_pool_info).value;

    vk::DescriptorSetAllocateInfo frame_descriptor_set_allocate_info = {
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_renderer->GetGraph()->GetFrameDescriptorSetLayout()
    };

    vk::DescriptorSetAllocateInfo camera_descriptor_set_allocate_info = {
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_renderer->GetGraph()->GetCameraDescriptorSetLayout()
    };

    m_frame_descriptor_set  = m_renderer->GetDevice()->GetLogicalDevice().allocateDescriptorSets(frame_descriptor_set_allocate_info) .value.front();
    m_camera_descriptor_set = m_renderer->GetDevice()->GetLogicalDevice().allocateDescriptorSets(camera_descriptor_set_allocate_info).value.front();

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

    m_renderer->GetDevice()->GetLogicalDevice().updateDescriptorSets(write_descriptor, VK_NULL_HANDLE);
}

RenderFrame::~RenderFrame() noexcept
{
    m_renderer->GetDevice()->GetLogicalDevice().destroy(m_descriptor_pool);
}

RkVoid RenderFrame::Reset() noexcept
{
    vk::SemaphoreWaitInfo semaphore_wait_info = {
        .semaphoreCount = 1U,
        .pSemaphores    = &m_timeline_semaphore.GetHandle(),
        .pValues        = &m_timeline_semaphore.GetValue ()
    };

    if (m_renderer->GetDevice()->GetLogicalDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX) != vk::Result::eSuccess)
    {

    }

    m_semaphore_pool       .Reset();
    m_graphics_command_pool.Reset();
    m_compute_command_pool .Reset();
    m_transfer_command_pool.Reset();
}

vk::DescriptorSet const& RenderFrame::GetFrameDescriptorSet() const noexcept
{
    return m_frame_descriptor_set;
}

vk::DescriptorSet const& RenderFrame::GetCameraDescriptorSet() const noexcept
{
    return m_camera_descriptor_set;
}

RkUint32 RenderFrame::GetIndex() const noexcept
{
    return m_index;
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

Buffer& RenderFrame::GetCameraUniformBuffer() noexcept
{
    return m_camera_uniform_buffer;
}
}
