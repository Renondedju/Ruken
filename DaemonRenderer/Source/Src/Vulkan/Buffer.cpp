/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Vulkan/Buffer.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

Buffer::Buffer(Device                  const& in_device,
               VkBufferCreateInfo      const& in_buffer_create_info,
               VmaAllocationCreateInfo const& in_allocation_create_info) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false},
    m_is_persistent     {(in_allocation_create_info.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) == 0}
{
    vmaCreateBuffer(m_device.GetAllocator(),
                    &in_buffer_create_info,
                    &in_allocation_create_info,
                    &m_handle,
                    &m_allocation,
                    &m_allocation_info);
}

Buffer::Buffer(Device                   const&  in_device,
               VkDeviceSize             const   in_size,
               VkBufferUsageFlags       const   in_buffer_usage,
               VmaAllocationCreateFlags const   in_flags,
               VmaMemoryUsage           const   in_memory_usage) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false},
    m_is_persistent     {(in_flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) == 0}
{
    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = in_buffer_usage;

    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = in_flags;
    allocation_create_info.usage = in_memory_usage;

    VmaAllocationInfo allocation_info;

    vmaCreateBuffer(m_device.GetAllocator(),
                    &buffer_create_info,
                    &allocation_create_info,
                    &m_handle,
                    &m_allocation,
                    &allocation_info);
}

Buffer::Buffer(Device                   const&  in_device,
               VkDeviceSize             const   in_size,
               VkBufferUsageFlags       const   in_buffer_usage,
               VmaAllocationCreateFlags const   in_flags,
               VmaMemoryUsage           const   in_memory_usage,
               std::vector<DAEuint32>   const&  in_queue_family_indices) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false},
    m_is_persistent     {(in_flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) == 0}
{
    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size                  = in_size;
    buffer_create_info.usage                 = in_buffer_usage;
    buffer_create_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
    buffer_create_info.queueFamilyIndexCount = static_cast<DAEuint32>(in_queue_family_indices.size());
    buffer_create_info.pQueueFamilyIndices   = in_queue_family_indices.data();

    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = in_flags;
    allocation_create_info.usage = in_memory_usage;

    vmaCreateBuffer(m_device.GetAllocator(),
                    &buffer_create_info,
                    &allocation_create_info,
                    &m_handle,
                    &m_allocation,
                    &m_allocation_info);
}

Buffer::Buffer(Buffer&& in_move) noexcept:
    m_device            {std::move(in_move.m_device)},
    m_handle            {in_move.m_handle},
    m_allocation        {in_move.m_allocation},
    m_allocation_info   {},
    m_is_mapped         {in_move.m_is_mapped},
    m_is_persistent     {in_move.m_is_persistent}
{
    in_move.m_handle          = nullptr;
    in_move.m_allocation      = nullptr;
    in_move.m_allocation_info = {};
    in_move.m_is_mapped       = false;
    in_move.m_is_persistent   = false;
}

Buffer::~Buffer() noexcept
{
    if (m_handle && m_allocation)
    {
        UnMap();

        vmaDestroyBuffer(m_device.GetAllocator(), m_handle, m_allocation);
    }
}

#pragma endregion

#pragma region Methods

DAEvoid* Buffer::Map() noexcept
{
    if (!m_is_persistent && !m_is_mapped && !m_allocation_info.pMappedData)
    {
        m_is_mapped = true;

        vmaMapMemory(m_device.GetAllocator(), m_allocation, &m_allocation_info.pMappedData);
    }

    return m_allocation_info.pMappedData;
}

DAEvoid Buffer::UnMap() noexcept
{
    if (m_is_persistent || !m_is_mapped || !m_allocation_info.pMappedData)
        return;

    m_is_mapped = false;

    vmaUnmapMemory(m_device.GetAllocator(), m_allocation);
}

DAEvoid Buffer::Flush() const noexcept
{
    vmaFlushAllocation(m_device.GetAllocator(), m_allocation, m_allocation_info.offset, m_allocation_info.size);
}

DAEvoid Buffer::Update(DAEvoid const* in_data, VkDeviceSize const in_offset, VkDeviceSize const in_size) noexcept
{
    if (m_is_persistent)
    {
        memcpy(m_allocation_info.pMappedData, in_data, in_size);

        Flush();
    }

    else
    {
        Map();

        memcpy(m_allocation_info.pMappedData, in_data, in_size);

        Flush();
        UnMap();
    }
}

DAEbool Buffer::IsMappable() const noexcept
{
    VkMemoryPropertyFlags flags;

    vmaGetMemoryTypeProperties(m_device.GetAllocator(), m_allocation_info.memoryType, &flags);

    return (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0;
}

Device const& Buffer::GetDevice() const noexcept
{
    return m_device;
}

VkBuffer const& Buffer::GetHandle() const noexcept
{
    return m_handle;
}

VmaAllocation const& Buffer::GetAllocation() const noexcept
{
    return m_allocation;
}

VkDeviceMemory const& Buffer::GetMemory() const noexcept
{
    return m_allocation_info.deviceMemory;
}

VkDeviceSize const& Buffer::GetOffset() const noexcept
{
    return m_allocation_info.offset;
}

VkDeviceSize const& Buffer::GetSize() const noexcept
{
    return m_allocation_info.size;
}

DAEvoid* Buffer::GetMappedData() const noexcept
{
    return m_allocation_info.pMappedData;
}

#pragma endregion