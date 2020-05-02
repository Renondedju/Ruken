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

#pragma region Constructors

VulkanImage::VulkanImage(VkImage          in_handle,
                         VkFormat   const in_format,
                         VkExtent3D const in_extent) noexcept:
    m_handle {in_handle},
    m_format {in_format},
    m_extent {in_extent}
{

}

VulkanImage::VulkanImage(VkImage          in_handle,
                         VmaAllocator     in_allocator,
                         VmaAllocation    in_allocation,
                         VkFormat   const in_format,
                         VkExtent3D const in_extent) noexcept:
    m_handle     {in_handle},
    m_allocator  {in_allocator},
    m_allocation {in_allocation},
    m_format     {in_format},
    m_extent     {in_extent}
{

}

VulkanImage::VulkanImage(VulkanImage&& in_move) noexcept:
    m_handle     {in_move.m_handle},
    m_allocator  {in_move.m_allocator},
    m_allocation {in_move.m_allocation},
    m_extent     {in_move.m_extent}
{
    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;
}

VulkanImage::~VulkanImage() noexcept
{
    if (!m_handle || !m_allocator || !m_allocation)
        return;

    vmaDestroyImage(m_allocator, m_handle, m_allocation);
}

#pragma endregion

#pragma region Methods

VkImage const& VulkanImage::GetHandle() const noexcept
{
    return m_handle;
}

VkImageType const& VulkanImage::GetType() const noexcept
{
    return m_type;
}

VkFormat const& VulkanImage::GetFormat() const noexcept
{
    return m_format;
}

VkExtent3D const& VulkanImage::GetExtent() const noexcept
{
    return m_extent;
}

#pragma endregion

#pragma region Operators

VulkanImage& VulkanImage::operator=(VulkanImage&& in_move) noexcept
{
    m_handle     = in_move.m_handle;
    m_allocator  = in_move.m_allocator;
    m_allocation = in_move.m_allocation;
    m_type       = in_move.m_type;
    m_format     = in_move.m_format;
    m_extent     = in_move.m_extent;

    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;

    return *this;
}

DAEbool DAEMON_NAMESPACE::operator==(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept
{
    return in_lhs.width  == in_rhs.width  &&
           in_rhs.height == in_lhs.height &&
           in_rhs.depth  == in_lhs.depth;
}

DAEbool DAEMON_NAMESPACE::operator!=(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept
{
    return in_lhs.width  != in_rhs.width  ||
           in_rhs.height != in_lhs.height ||
           in_rhs.depth  != in_lhs.depth;
}

#pragma endregion