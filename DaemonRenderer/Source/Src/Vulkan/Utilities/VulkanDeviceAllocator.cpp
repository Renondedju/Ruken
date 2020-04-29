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

#pragma warning (push, 0)

#define VMA_IMPLEMENTATION

#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

#pragma warning (pop)

#include "Vulkan/Core/VulkanPhysicalDevice.hpp"
#include "Vulkan/Core/VulkanDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors

VulkanDeviceAllocator::VulkanDeviceAllocator(VulkanPhysicalDevice const& in_physical_device,
                                             VulkanDevice         const& in_device) noexcept
{
    VmaVulkanFunctions functions = {
        vkGetPhysicalDeviceProperties,
        vkGetPhysicalDeviceMemoryProperties,
        vkAllocateMemory,
        vkFreeMemory,
        vkMapMemory,
        vkUnmapMemory,
        vkFlushMappedMemoryRanges,
        vkInvalidateMappedMemoryRanges,
        vkBindBufferMemory,
        vkBindImageMemory,
        vkGetBufferMemoryRequirements,
        vkGetImageMemoryRequirements,
        vkCreateBuffer,
        vkDestroyBuffer,
        vkCreateImage,
        vkDestroyImage,
        vkCmdCopyBuffer,
        vkGetBufferMemoryRequirements2KHR,
        vkGetImageMemoryRequirements2KHR,
        vkBindBufferMemory2KHR,
        vkBindImageMemory2KHR,
        vkGetPhysicalDeviceMemoryProperties2KHR
    };

    VmaAllocatorCreateInfo allocator_create_info = {};

    allocator_create_info.physicalDevice   = in_physical_device.GetHandle();
    allocator_create_info.device           = in_device.GetHandle();
    allocator_create_info.pVulkanFunctions = &functions;

    VK_CHECK(vmaCreateAllocator(&allocator_create_info, &m_handle));
}

VulkanDeviceAllocator::~VulkanDeviceAllocator() noexcept
{
    if (!m_handle)
        return;

    vmaDestroyAllocator(m_handle);
}

#pragma endregion

#pragma region Methods

std::optional<VulkanImage> VulkanDeviceAllocator::CreateImage(VkImageCreateInfo       const& in_image_create_info,
                                                              VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept
{
    VkImage           image           = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (VK_CHECK(vmaCreateImage(m_handle, &in_image_create_info, &in_allocation_create_info, &image, &allocation, &allocation_info)))
        return std::nullopt;

    return VulkanImage(image, m_handle, allocation, allocation_info);
}

std::optional<VulkanBuffer> VulkanDeviceAllocator::CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                                                                VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept
{
    VkBuffer          buffer          = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (VK_CHECK(vmaCreateBuffer(m_handle, &in_buffer_create_info, &in_allocation_create_info, &buffer, &allocation, &allocation_info)))
        return std::nullopt;

    return VulkanBuffer(buffer,
                        m_handle,
                        allocation,
                        allocation_info,
                        (in_allocation_create_info.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) == 0);
}

#pragma endregion