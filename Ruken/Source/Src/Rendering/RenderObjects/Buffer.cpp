#include "Rendering/RenderObjects/Buffer.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Buffer::Buffer(RenderDevice* in_device, VkDeviceSize in_size, vk::BufferUsageFlags in_usage) noexcept:
    m_device {in_device}
{
    if (in_size == 0ULL)
        return;

    vk::AllocationCreateInfo allocation_create_info = {};

    if (in_usage & vk::BufferUsageFlagBits::eVertexBuffer || in_usage & vk::BufferUsageFlagBits::eIndexBuffer)
    {
        in_usage |= vk::BufferUsageFlagBits::eTransferDst;

        allocation_create_info.usage = vk::MemoryUsage::eGpuOnly;
    }

    else if (in_usage & vk::BufferUsageFlagBits::eUniformBuffer || in_usage & vk::BufferUsageFlagBits::eStorageBuffer)
    {
        allocation_create_info.usage = vk::MemoryUsage::eCpuToGpu;
    }

    else if (in_usage & vk::BufferUsageFlagBits::eTransferSrc)
    {
        allocation_create_info.usage = vk::MemoryUsage::eCpuOnly;
    }

    vk::BufferCreateInfo buffer_create_info = {
        .size  = in_size,
        .usage = in_usage,
    };

    auto [result, value] = m_device->GetAllocator().createBuffer(buffer_create_info, allocation_create_info, m_allocation_info);

    std::tie(m_handle, m_allocation) = value;
}

Buffer::~Buffer() noexcept
{
    m_device->GetAllocator().destroyBuffer(m_handle, m_allocation);
}

RkVoid Buffer::Resize(VkDeviceSize in_size, vk::BufferUsageFlags in_usage) noexcept
{
    if (m_handle)
        return;

    vk::AllocationCreateInfo allocation_create_info = {};

    if (in_usage & vk::BufferUsageFlagBits::eVertexBuffer || in_usage & vk::BufferUsageFlagBits::eIndexBuffer)
    {
        in_usage |= vk::BufferUsageFlagBits::eTransferDst;

        allocation_create_info.usage = vk::MemoryUsage::eGpuOnly;
    }

    else if (in_usage & vk::BufferUsageFlagBits::eUniformBuffer || in_usage & vk::BufferUsageFlagBits::eStorageBuffer)
    {
        allocation_create_info.usage = vk::MemoryUsage::eCpuToGpu;
    }

    else if (in_usage & vk::BufferUsageFlagBits::eTransferSrc)
    {
        allocation_create_info.usage = vk::MemoryUsage::eCpuOnly;
    }

    vk::BufferCreateInfo buffer_create_info = {
        .size  = in_size,
        .usage = in_usage,
    };

    auto [result, value] = m_device->GetAllocator().createBuffer(buffer_create_info, allocation_create_info, m_allocation_info);

    std::tie(m_handle, m_allocation) = value;
}

RkVoid* Buffer::Map() const noexcept
{
    return m_device->GetAllocator().mapMemory(m_allocation).value;
}

RkVoid Buffer::UnMap() const noexcept
{
    m_device->GetAllocator().unmapMemory(m_allocation);
}

vk::Buffer const& Buffer::GetHandle() const noexcept
{
    return m_handle;
}
