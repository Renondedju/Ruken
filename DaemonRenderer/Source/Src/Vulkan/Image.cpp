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

#include "Vulkan/Image.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Image::Image(Device  const& in_device,
             VkImage const  in_handle) noexcept:
    m_device            {in_device},
    m_handle            {in_handle},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false}
{

}

Image::Image(Device                  const& in_device,
             VkImageCreateInfo       const& in_image_create_info,
             VmaAllocationCreateInfo const& in_allocation_create_info) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false}
{
    vmaCreateImage(m_device.GetAllocator(),
                   &in_image_create_info,
                   &in_allocation_create_info,
                   &m_handle,
                   &m_allocation,
                   &m_allocation_info);
}

Image::Image(Device                   const&    in_device,
             VkImageType              const     in_image_type,
             VkFormat                 const     in_format,
             VkExtent3D               const     in_extent,
             DAEuint32                const     in_mip_levels,
             DAEuint32                const     in_array_layers,
             VkImageTiling            const     in_tiling,
             VkSampleCountFlagBits    const     in_sample_count,
             VkImageUsageFlags        const     in_image_usage,
             VmaAllocationCreateFlags const     in_flags,
             VmaMemoryUsage           const     in_memory_usage) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false}
{
    VkImageCreateInfo image_create_info = {};

    image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType     = in_image_type;
    image_create_info.format        = in_format;
    image_create_info.extent        = in_extent;
    image_create_info.mipLevels     = in_mip_levels;
    image_create_info.arrayLayers   = in_array_layers;
    image_create_info.samples       = in_sample_count;
    image_create_info.tiling        = in_tiling;
    image_create_info.usage         = in_image_usage;

    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = in_flags;
    allocation_create_info.usage = in_memory_usage;

    if (in_image_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
        allocation_create_info.preferredFlags = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

    vmaCreateImage(m_device.GetAllocator(),
                   &image_create_info,
                   &allocation_create_info,
                   &m_handle,
                   &m_allocation,
                   &m_allocation_info);
}

Image::Image(Device                   const&    in_device,
             VkImageType              const     in_image_type,
             VkFormat                 const     in_format,
             VkExtent3D               const     in_extent,
             DAEuint32                const     in_mip_levels,
             DAEuint32                const     in_array_layers,
             VkImageTiling            const     in_tiling,
             VkSampleCountFlagBits    const     in_sample_count,
             VkImageUsageFlags        const     in_image_usage,
             VmaAllocationCreateFlags const     in_flags,
             VmaMemoryUsage           const     in_memory_usage,
             std::vector<DAEuint32>   const&    in_queue_family_indices) noexcept:
    m_device            {in_device},
    m_handle            {nullptr},
    m_allocation        {nullptr},
    m_allocation_info   {},
    m_is_mapped         {false}
{
    VkImageCreateInfo image_create_info = {};

    image_create_info.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType             = in_image_type;
    image_create_info.format                = in_format;
    image_create_info.extent                = in_extent;
    image_create_info.mipLevels             = in_mip_levels;
    image_create_info.arrayLayers           = in_array_layers;
    image_create_info.samples               = in_sample_count;
    image_create_info.tiling                = in_tiling;
    image_create_info.usage                 = in_image_usage;
    image_create_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
    image_create_info.queueFamilyIndexCount = static_cast<DAEuint32>(in_queue_family_indices.size());
    image_create_info.pQueueFamilyIndices   = in_queue_family_indices.data();

    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = in_flags;
    allocation_create_info.usage = in_memory_usage;

    if (in_image_usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
        allocation_create_info.preferredFlags = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

    vmaCreateImage(m_device.GetAllocator(),
                   &image_create_info,
                   &allocation_create_info,
                   &m_handle,
                   &m_allocation,
                   &m_allocation_info);
}

Image::Image(Image&& in_move) noexcept:
    m_device            {std::move(in_move.m_device)},
    m_handle            {in_move.m_handle},
    m_allocation        {in_move.m_allocation},
    m_allocation_info   {in_move.m_allocation_info},
    m_is_mapped         {in_move.m_is_mapped}
{
    in_move.m_handle          = nullptr;
    in_move.m_allocation      = nullptr;
    in_move.m_allocation_info = {};
    in_move.m_is_mapped       = false;
}

Image::~Image() noexcept
{
    if (!m_handle || !m_allocation)
        return;

    UnMap();

    vmaDestroyImage(m_device.GetAllocator(), m_handle, m_allocation);
}

#pragma endregion

#pragma region Methods

DAEvoid* Image::Map() noexcept
{
    if (!m_is_mapped && !m_allocation_info.pMappedData)
    {
        m_is_mapped = true;

        vmaMapMemory(m_device.GetAllocator(), m_allocation, &m_allocation_info.pMappedData);
    }

    return m_allocation_info.pMappedData;
}

DAEvoid Image::UnMap() noexcept
{
    if (!m_is_mapped || !m_allocation_info.pMappedData)
        return;

    m_is_mapped = false;

    vmaUnmapMemory(m_device.GetAllocator(), m_allocation);
}

Device const& Image::GetDevice() const noexcept
{
    return m_device;
}

VkImage const& Image::GetHandle() const noexcept
{
    return m_handle;
}

VmaAllocation const& Image::GetAllocation() const noexcept
{
    return m_allocation;
}

VkDeviceMemory const& Image::GetMemory() const noexcept
{
    return m_allocation_info.deviceMemory;
}

DAEvoid* Image::GetMappedData() const noexcept
{
    return m_allocation_info.pMappedData;
}

#pragma endregion