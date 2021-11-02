#include "Rendering/RenderObjects/Buffer.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

Buffer::Buffer(RenderDevice* in_device, vk::DeviceSize in_size, vk::BufferUsageFlags in_usage) noexcept:
    m_device {in_device},
    m_size   {in_size},
    m_usage  {in_usage}
{
    if (m_size == 0ULL)
        return;

    vk::BufferCreateInfo buffer_create_info = {
        .size  = m_size,
        .usage = m_usage
    };

    VmaAllocationCreateInfo allocation_create_info = {};

    if (m_usage & vk::BufferUsageFlagBits::eTransferSrc)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    }

    else if (m_usage & vk::BufferUsageFlagBits::eTransferDst)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    }
    
    else
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(m_device->GetAllocator(), reinterpret_cast<VkBufferCreateInfo*>(&buffer_create_info), &allocation_create_info, reinterpret_cast<VkBuffer*>(&m_handle), &m_allocation, &m_allocation_info);
}

Buffer::Buffer(RenderDevice* in_device, vk::BufferCreateInfo const& in_buffer_create_info) noexcept:
    m_device {in_device},
    m_size   {in_buffer_create_info.size},
    m_usage  {in_buffer_create_info.usage}
{
    if (m_size == 0ULL)
        return;

    VmaAllocationCreateInfo allocation_create_info = {};

    if (m_usage & vk::BufferUsageFlagBits::eTransferSrc)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    }

    else if (m_usage & vk::BufferUsageFlagBits::eTransferDst)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    }
    
    else
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(m_device->GetAllocator(), reinterpret_cast<VkBufferCreateInfo const*>(&in_buffer_create_info), &allocation_create_info, reinterpret_cast<VkBuffer*>(&m_handle), &m_allocation, &m_allocation_info);
}

Buffer::~Buffer() noexcept
{
    vmaDestroyBuffer(m_device->GetAllocator(), m_handle, m_allocation);
}

RkVoid Buffer::Resize(VkDeviceSize const in_size) noexcept
{
    if (m_handle)
        vmaDestroyBuffer(m_device->GetAllocator(), m_handle, m_allocation);

    m_size = in_size;

    if (m_size == 0ULL)
        return;

    vk::BufferCreateInfo buffer_create_info = {
        .size  = m_size,
        .usage = m_usage
    };

    VmaAllocationCreateInfo allocation_create_info = {};

    if (m_usage & vk::BufferUsageFlagBits::eTransferSrc)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    }

    else if (m_usage & vk::BufferUsageFlagBits::eTransferDst)
    {
        allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    }
    
    else
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(m_device->GetAllocator(), reinterpret_cast<VkBufferCreateInfo*>(&buffer_create_info), &allocation_create_info, reinterpret_cast<VkBuffer*>(&m_handle), &m_allocation, &m_allocation_info);
}

RkVoid* Buffer::Map() const noexcept
{
    RkVoid* data;

    vmaMapMemory(m_device->GetAllocator(), m_allocation, &data);

    return data;
}

RkVoid Buffer::UnMap() const noexcept
{
    vmaUnmapMemory(m_device->GetAllocator(), m_allocation);
}

vk::Buffer const& Buffer::GetHandle() const noexcept
{
    return m_handle;
}