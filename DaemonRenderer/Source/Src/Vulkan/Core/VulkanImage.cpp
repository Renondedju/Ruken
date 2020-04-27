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

#include "Vulkan/Core/VulkanImage.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

VulkanImage::VulkanImage(VkImage in_handle) noexcept:
    m_handle {in_handle}
{

}

VulkanImage::VulkanImage(VkImage            in_handle,
                         VmaAllocator       in_allocator,
                         VmaAllocation      in_allocation,
                         VmaAllocationInfo  in_allocation_info) noexcept:
    m_handle            {in_handle},
    m_allocator         {in_allocator},
    m_allocation        {in_allocation},
    m_allocation_info   {in_allocation_info}
{

}

VulkanImage::VulkanImage(VulkanImage&& in_move) noexcept:
    m_handle            {in_move.m_handle},
    m_allocator         {in_move.m_allocator},
    m_allocation        {in_move.m_allocation},
    m_allocation_info   {in_move.m_allocation_info},
    m_is_mapped         {in_move.m_is_mapped}
{
    in_move.m_handle          = nullptr;
    in_move.m_allocator       = nullptr;
    in_move.m_allocation      = nullptr;
    in_move.m_allocation_info = {};
    in_move.m_is_mapped       = false;
}

VulkanImage::~VulkanImage() noexcept
{
    if (m_handle && m_allocator && m_allocation)
    {
        UnMap();

        vmaDestroyImage(m_allocator, m_handle, m_allocation);
    }
}

#pragma endregion

#pragma region Methods

DAEvoid* VulkanImage::Map() noexcept
{
    if (!m_is_mapped && !m_allocation_info.pMappedData)
    {
        m_is_mapped = true;

        vmaMapMemory(m_allocator, m_allocation, &m_allocation_info.pMappedData);
    }

    return m_allocation_info.pMappedData;
}

DAEvoid VulkanImage::UnMap() noexcept
{
    if (!m_is_mapped || !m_allocation_info.pMappedData)
        return;

    m_is_mapped = false;

    vmaUnmapMemory(m_allocator, m_allocation);
}

VkImage const& VulkanImage::GetHandle() const noexcept
{
    return m_handle;
}

VmaAllocation const& VulkanImage::GetAllocation() const noexcept
{
    return m_allocation;
}

VkDeviceMemory const& VulkanImage::GetMemory() const noexcept
{
    return m_allocation_info.deviceMemory;
}

DAEvoid* VulkanImage::GetMappedData() const noexcept
{
    return m_allocation_info.pMappedData;
}

VkExtent3D const& VulkanImage::GetExtent() const noexcept
{
    return m_extent;
}

#pragma endregion

#pragma region Operator

VulkanImage& VulkanImage::operator=(VulkanImage&& in_move) noexcept
{
    m_handle            = in_move.m_handle;
    m_allocator         = in_move.m_allocator;
    m_allocation        = in_move.m_allocation;
    m_allocation_info   = in_move.m_allocation_info;
    m_is_mapped         = in_move.m_is_mapped;

    in_move.m_handle          = nullptr;
    in_move.m_allocator       = nullptr;
    in_move.m_allocation      = nullptr;
    in_move.m_allocation_info = {};
    in_move.m_is_mapped       = false;

    return *this;
}

#pragma endregion