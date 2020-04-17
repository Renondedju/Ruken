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

#include <map>
#include <set>

#pragma warning (push, 0)

#define VMA_IMPLEMENTATION

#include "Vulkan/Core/Device.hpp"

#pragma warning (pop)

#include "Vulkan/Core/Instance.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanException.hpp"

USING_DAEMON_NAMESPACE

#pragma region Static Variables

static std::vector<DAEchar const*> RequiredExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructor and Destructor

Device::Device(Instance const& in_instance)
{
    if (!PickPhysicalDevice(in_instance.GetPhysicalDevices()))
        throw VulkanException("No suitable GPU!");

    if (!CreateDevice())
        throw VulkanException("Failed to create device!");

    VulkanLoader::LoadDevice(m_handle);

    if (!CreateMemoryAllocator())
        throw VulkanException("Failed to create device memory allocator!");

    SetupQueues();

    m_pipeline_cache = std::make_unique<PipelineCache>();
}

Device::~Device() noexcept
{
    m_pipeline_cache.reset();

    if (m_allocator)
        vmaDestroyAllocator(m_allocator);

    if (m_handle)
        vkDestroyDevice(m_handle, nullptr);

    m_handle    = nullptr;
    m_allocator = nullptr;
}

#pragma endregion

#pragma region Methods

DAEbool Device::CheckDeviceExtensions(VkPhysicalDevice in_physical_device) noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of available physical device extensions.
    if (!VK_CHECK(vkEnumerateDeviceExtensionProperties(in_physical_device, nullptr, &count, nullptr)))
        return false;

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the available physical device extensions.
    if (!VK_CHECK(vkEnumerateDeviceExtensionProperties(in_physical_device, nullptr, &count, supported_extensions.data())))
        return false;

    std::set<std::string> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);

        if (required_extensions.empty())
            return true;
    }

    return false;
}

DAEuint32 Device::RateDeviceSuitability(PhysicalDevice const& in_physical_device) noexcept
{
    if (!CheckDeviceExtensions(in_physical_device.GetHandle()))
        return 0u;

    auto const& properties = in_physical_device.GetProperties();

    if (properties.apiVersion < VK_API_VERSION_1_2)
        return 0u;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return 1000u;

    return 0u;
}

DAEbool Device::PickPhysicalDevice(std::vector<PhysicalDevice> const& in_physical_devices) noexcept
{
    // Uses an ordered map to automatically sort candidates by increasing score.
    std::multimap<DAEuint32, PhysicalDevice const*> candidates;

    for (auto const& physical_device : in_physical_devices)
        candidates.insert(std::make_pair(RateDeviceSuitability(physical_device), &physical_device));

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (candidate.first > 0u)
        {
            m_physical_device = candidate.second;

            return true;
        }
    }

    return false;
}

DAEbool Device::CreateDevice()
{
    auto const& queue_family_properties = m_physical_device->GetQueueFamilyProperties();

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_properties.size());

    std::vector<std::vector<DAEfloat>> priorities(queue_family_properties.size());

    for (DAEuint32 i = 0; i < static_cast<DAEuint32>(queue_family_properties.size()); ++i)
    {
        priorities[i].resize(queue_family_properties[i].queueCount, 1.0f);

        queue_create_infos[i].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = i;
        queue_create_infos[i].queueCount       = queue_family_properties[i].queueCount;
        queue_create_infos[i].pQueuePriorities = priorities[i].data();
    }

    VkDeviceCreateInfo device_create_info = {};

    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount    = static_cast<DAEuint32>(queue_create_infos.size());
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.enabledExtensionCount   = static_cast<DAEuint32>(RequiredExtensions.size());
    device_create_info.ppEnabledExtensionNames = RequiredExtensions.data();

    return VK_CHECK(vkCreateDevice(m_physical_device->GetHandle(), &device_create_info, nullptr, &m_handle));
}

DAEbool Device::CreateMemoryAllocator() noexcept
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

    allocator_create_info.physicalDevice   = m_physical_device->GetHandle();
    allocator_create_info.device           = m_handle;
    allocator_create_info.pVulkanFunctions = &functions;

    return VK_CHECK(vmaCreateAllocator(&allocator_create_info, &m_allocator));
}

DAEvoid Device::SetupQueues()
{
    auto const& queue_family_properties = m_physical_device->GetQueueFamilyProperties();

    m_queues.resize(queue_family_properties.size());

    for (DAEuint32 i = 0; i < static_cast<DAEuint32>(queue_family_properties.size()); ++i)
    {
        for (DAEuint32 j = 0; j < queue_family_properties[i].queueCount; ++j)
        {
            VkQueue queue = nullptr;

            vkGetDeviceQueue(m_handle, i, j, &queue);

            if (queue)
                m_queues[i].emplace_back(m_physical_device, queue, i, j, queue_family_properties[i].queueFlags);
        }
    }
}

DAEvoid Device::WaitIdle() const noexcept
{
    VK_CHECK(vkDeviceWaitIdle(m_handle));
}

std::optional<Image> Device::CreateImage(VkImageCreateInfo       const& in_image_create_info,
                                         VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept
{
    VkImage           image           = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (VK_CHECK(vmaCreateImage(m_allocator, &in_image_create_info, &in_allocation_create_info, &image, &allocation, &allocation_info)))
        return Image(image, m_allocator, allocation, allocation_info);

    return std::nullopt;
}

std::optional<Buffer> Device::CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                                           VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept
{
    VkBuffer          buffer          = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (VK_CHECK(vmaCreateBuffer(m_allocator, &in_buffer_create_info, &in_allocation_create_info, &buffer, &allocation, &allocation_info)))
    {
        DAEbool const is_persistent = (in_allocation_create_info.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) == 0;

        return Buffer(buffer, m_allocator, allocation, allocation_info, is_persistent);
    }

    return std::nullopt;
}

std::optional<Queue const*> Device::RequestGraphicsQueue() const noexcept
{
    for (auto const& queue_family : m_queues)
    {
        for (auto const& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_GRAPHICS_BIT)
                return &queue;
        }
    }

    return std::nullopt;
}

std::optional<Queue const*> Device::RequestComputeQueue() const noexcept
{
    for (auto const& queue_family : m_queues)
    {
        for (auto const& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_COMPUTE_BIT)
                return &queue;
        }
    }

    return std::nullopt;
}

std::optional<Queue const*> Device::RequestTransferQueue() const noexcept
{
    for (auto const& queue_family : m_queues)
    {
        for (auto const& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_TRANSFER_BIT)
                return &queue;
        }
    }

    return std::nullopt;
}

std::optional<Queue const*> Device::RequestPresentQueue(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue_family : m_queues)
    {
        for (auto const& queue : queue_family)
        {
            if (queue.IsPresentationSupported(in_surface))
                return &queue;
        }
    }

    return std::nullopt;
}

PhysicalDevice const* Device::GetPhysicalDevice() const noexcept
{
    return m_physical_device;
}

VkDevice const& Device::GetHandle() const noexcept
{
    return m_handle;
}

PipelineCache& Device::GetPipelineCache() const noexcept
{
    return *m_pipeline_cache;
}

#pragma endregion