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

#include "Vulkan/Core/VulkanBuffer.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanBuffer::VulkanBuffer(VkBuffer          in_handle,
                           VmaAllocator      in_allocator,
                           VmaAllocation     in_allocation,
                           VmaAllocationInfo in_allocation_info,
                           RkBool const     in_persistent) noexcept:
    m_handle          {in_handle},
    m_allocator       {in_allocator},
    m_allocation      {in_allocation},
    m_allocation_info {in_allocation_info},
    m_is_persistent   {in_persistent},
    m_is_mapped       {in_persistent}
{

}

VulkanBuffer::VulkanBuffer(VulkanBuffer&& in_move) noexcept:
    m_handle          {in_move.m_handle},
    m_allocator       {in_move.m_allocator},
    m_allocation      {in_move.m_allocation},
    m_allocation_info {in_move.m_allocation_info},
    m_is_persistent   {in_move.m_is_persistent},
    m_is_mapped       {in_move.m_is_mapped}
{
    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;
}

VulkanBuffer::~VulkanBuffer() noexcept
{
    if (!m_handle || !m_allocator || !m_allocation)
        return;

    vmaDestroyBuffer(m_allocator, m_handle, m_allocation);
}

#pragma endregion

#pragma region Methods

RkVoid* VulkanBuffer::Map() noexcept
{
    // Mapping persistent or already mapped data does nothing.
    if (!m_is_persistent && !m_is_mapped)
    {
        if (VK_CHECK(vmaMapMemory(m_allocator, m_allocation, &m_allocation_info.pMappedData)))
            return nullptr;

        m_is_mapped = true;
    }

    if (VK_CHECK(vmaInvalidateAllocation(m_allocator, m_allocation, 0ull, m_allocation_info.size)))
        return nullptr;

    return m_allocation_info.pMappedData;
}

RkBool VulkanBuffer::Update(RkVoid const* in_data, RkSize const in_size) noexcept
{
    // Persistent data is always mapped.
    if (m_is_persistent)
    {
        memcpy(m_allocation_info.pMappedData, in_data, in_size);

        return true;
    }

    if (!Map())
        return false;

    memcpy(m_allocation_info.pMappedData, in_data, in_size);

    return Unmap();
}

RkBool VulkanBuffer::Unmap() noexcept
{
    // Unmapping persistent or already unmapped data does nothing.
    if (!m_is_persistent && m_is_mapped)
    {
        vmaUnmapMemory(m_allocator, m_allocation);
        
        m_is_mapped = false;
    }

    if (VK_CHECK(vmaFlushAllocation(m_allocator, m_allocation, 0ull, m_allocation_info.size)))
        return false;

    return true;
}

VkBuffer const& VulkanBuffer::GetHandle() const noexcept
{
    return m_handle;
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

RkBool VulkanBuffer::IsMapped() const noexcept
{
    return m_is_mapped;
}

#pragma endregion

#pragma region Operators

VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& in_move) noexcept
{
    m_handle          = in_move.m_handle;
    m_allocator       = in_move.m_allocator;
    m_allocation      = in_move.m_allocation;
    m_allocation_info = in_move.m_allocation_info;
    m_is_persistent   = in_move.m_is_persistent;
    m_is_mapped       = in_move.m_is_mapped;

    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;

    return *this;
}

#pragma endregion