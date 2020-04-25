/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#include <memory>

#include "Vulkan/Core/VulkanBuffer.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

VulkanBuffer::VulkanBuffer(VkBuffer in_handle) noexcept:
    m_handle {in_handle}
{

}

VulkanBuffer::VulkanBuffer(VkBuffer             in_handle,
                           VmaAllocator         in_allocator,
                           VmaAllocation        in_allocation,
                           VmaAllocationInfo    in_allocation_info,
                           DAEbool const        in_persistent) noexcept:
    m_handle            {in_handle},
    m_allocator         {in_allocator},
    m_allocation        {in_allocation},
    m_allocation_info   {in_allocation_info},
    m_is_persistent     {in_persistent}
{

}

VulkanBuffer::VulkanBuffer(VulkanBuffer&& in_move) noexcept:
    m_handle            {in_move.m_handle},
    m_allocator         {in_move.m_allocator},
    m_allocation        {in_move.m_allocation},
    m_allocation_info   {in_move.m_allocation_info},
    m_is_mapped         {in_move.m_is_mapped},
    m_is_persistent     {in_move.m_is_persistent}
{
    in_move.m_handle          = nullptr;
    in_move.m_allocator       = nullptr;
    in_move.m_allocation      = nullptr;
    in_move.m_allocation_info = {};
    in_move.m_is_mapped       = false;
    in_move.m_is_persistent   = false;
}

VulkanBuffer::~VulkanBuffer() noexcept
{
    if (!m_handle || !m_allocator || !m_allocation)
        return;

    UnMap();

    vmaDestroyBuffer(m_allocator, m_handle, m_allocation);
}

#pragma endregion

#pragma region Methods

DAEvoid* VulkanBuffer::Map() noexcept
{
    if (!m_is_persistent && !m_is_mapped && !m_allocation_info.pMappedData)
    {
        m_is_mapped = true;

        vmaMapMemory(m_allocator, m_allocation, &m_allocation_info.pMappedData);
    }

    return m_allocation_info.pMappedData;
}

DAEvoid VulkanBuffer::UnMap() noexcept
{
    if (m_is_persistent || !m_is_mapped || !m_allocation_info.pMappedData)
        return;

    m_is_mapped = false;

    vmaUnmapMemory(m_allocator, m_allocation);
}

DAEvoid VulkanBuffer::Flush() const noexcept
{
    vmaFlushAllocation(m_allocator, m_allocation, m_allocation_info.offset, m_allocation_info.size);
}

DAEvoid VulkanBuffer::Update(DAEvoid const* in_data, VkDeviceSize const in_offset, VkDeviceSize const in_size) noexcept
{
    (void)in_offset;

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

DAEbool VulkanBuffer::IsMappable() const noexcept
{
    VkMemoryPropertyFlags flags;

    vmaGetMemoryTypeProperties(m_allocator, m_allocation_info.memoryType, &flags);

    return (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0;
}

VkBuffer const& VulkanBuffer::GetHandle() const noexcept
{
    return m_handle;
}

VmaAllocation const& VulkanBuffer::GetAllocation() const noexcept
{
    return m_allocation;
}

VkDeviceMemory const& VulkanBuffer::GetMemory() const noexcept
{
    return m_allocation_info.deviceMemory;
}

VkDeviceSize const& VulkanBuffer::GetOffset() const noexcept
{
    return m_allocation_info.offset;
}

VkDeviceSize const& VulkanBuffer::GetSize() const noexcept
{
    return m_allocation_info.size;
}

DAEvoid* VulkanBuffer::GetMappedData() const noexcept
{
    return m_allocation_info.pMappedData;
}

#pragma endregion