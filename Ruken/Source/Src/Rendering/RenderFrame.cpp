#include "Rendering/RenderFrame.hpp"
#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

RenderFrame::RenderFrame(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph) noexcept:
    m_logger                   {in_logger},
    m_device                   {in_device},
    m_graph                    {in_graph},
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
    std::vector<vk::DescriptorPoolSize> pool_sizes = {
        {
            .type            = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 1000U
        },
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
        .maxSets       = static_cast<RkUint32>(pool_sizes.size()),
        .poolSizeCount = static_cast<RkUint32>(pool_sizes.size()),
        .pPoolSizes    = pool_sizes.data()
    };

    m_descriptor_pool = m_device->GetLogicalDevice().createDescriptorPool(descriptor_pool_info).value;

    vk::DescriptorSetVariableDescriptorCountAllocateInfo descriptor_set_variable_descriptor_count_allocate_info = {
        .descriptorSetCount = 1U,
        .pDescriptorCounts  = &pool_sizes[0].descriptorCount
    };

    vk::DescriptorSetAllocateInfo texture_descriptor_set_allocate_info = {
        .pNext              = &descriptor_set_variable_descriptor_count_allocate_info,
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_graph->GetTextureDescriptorSetLayout()
    };

    vk::DescriptorSetAllocateInfo frame_descriptor_set_allocate_info = {
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_graph->GetFrameDescriptorSetLayout()
    };

    vk::DescriptorSetAllocateInfo camera_descriptor_set_allocate_info = {
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_graph->GetCameraDescriptorSetLayout()
    };

    m_texture_descriptor_set = m_device->GetLogicalDevice().allocateDescriptorSets(texture_descriptor_set_allocate_info).value.front();
    m_frame_descriptor_set   = m_device->GetLogicalDevice().allocateDescriptorSets(frame_descriptor_set_allocate_info)  .value.front();
    m_camera_descriptor_set  = m_device->GetLogicalDevice().allocateDescriptorSets(camera_descriptor_set_allocate_info) .value.front();

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
}

RenderFrame::~RenderFrame() noexcept
{
    m_device->GetLogicalDevice().destroy(m_descriptor_pool);
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

vk::DescriptorSet const& RenderFrame::GetTextureDescriptorSet() const noexcept
{
    return m_texture_descriptor_set;
}

vk::DescriptorSet const& RenderFrame::GetFrameDescriptorSet() const noexcept
{
    return m_frame_descriptor_set;
}

vk::DescriptorSet const& RenderFrame::GetCameraDescriptorSet() const noexcept
{
    return m_camera_descriptor_set;
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
