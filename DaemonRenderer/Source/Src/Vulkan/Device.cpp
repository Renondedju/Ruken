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

#include <map>
#include <set>

#include "Vulkan/Image.hpp"
#include "Vulkan/Queue.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/PipelineCache.hpp"
#include "Vulkan/PhysicalDevice.hpp"

#include "Rendering/Renderer.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

#pragma region Static Variables

static std::vector<DAEchar const*> RequiredExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructor and Destructor

Device::Device(PhysicalDevice const& in_physical_devices):
    m_physical_device   {in_physical_devices},
    m_handle            {nullptr},
    m_allocator         {nullptr}
{
    if (SetupDevice())
    {
        GRenderer->GetLogger()->Info("Device created successfully.");

        volkLoadDevice(m_handle);

        if (SetupMemoryAllocator())
        {
            GRenderer->GetLogger()->Info("Device memory allocator created successfully.");

            SetupQueues();

            m_pipeline_cache = std::make_unique<PipelineCache>(*this);
        }

        else
            GRenderer->GetLogger()->Fatal("Failed to create device memory allocator!");
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create logical device!");
}

Device::~Device() noexcept
{
    m_pipeline_cache.reset();

    if (m_allocator)
        vmaDestroyAllocator(m_allocator);

    if (m_handle)
        vkDestroyDevice(m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

DAEbool Device::CheckDeviceExtensions(PhysicalDevice const& in_physical_device) noexcept
{
    DAEuint32 count;

    // Returns the number of available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_physical_device.GetHandle(), nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_physical_device.GetHandle(), nullptr, &count, supported_extensions.data());

    std::set<std::string> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
        required_extensions.erase(extension.extensionName);

    return required_extensions.empty();
}

DAEuint32 Device::RateDeviceSuitability(PhysicalDevice const& in_physical_device) noexcept
{
    if (!CheckDeviceExtensions(in_physical_device))
        return 0u;

    VkPhysicalDeviceProperties properties;

    vkGetPhysicalDeviceProperties(in_physical_device.GetHandle(), &properties);

    if (properties.apiVersion < VK_API_VERSION_1_1)
        return 0u;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return 1000u;

    return 0u;
}

DAEbool Device::SetupDevice()
{
    auto const& queue_family_properties = m_physical_device.GetQueueFamilyProperties();

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_properties.size());

    std::vector<std::vector<DAEfloat>> priorities(queue_family_properties.size());

    for (DAEsize i = 0; i < queue_family_properties.size(); ++i)
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

    return vkCreateDevice(m_physical_device.GetHandle(), &device_create_info, nullptr, &m_handle) == VK_SUCCESS;
}

DAEbool Device::SetupMemoryAllocator() noexcept
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

    allocator_create_info.physicalDevice   = m_physical_device.GetHandle();
    allocator_create_info.device           = m_handle;
    allocator_create_info.pVulkanFunctions = &functions;

    return vmaCreateAllocator(&allocator_create_info, &m_allocator) == VK_SUCCESS;
}

DAEvoid Device::SetupQueues()
{
    auto const& queue_family_properties = m_physical_device.GetQueueFamilyProperties();

    m_queues.resize(queue_family_properties.size());

    VkQueue queue;

    for (DAEsize i = 0; i < queue_family_properties.size(); ++i)
    {
        for (DAEuint32 j = 0u; j < queue_family_properties[i].queueCount; ++j)
        {
            vkGetDeviceQueue(m_handle, i, j, &queue);

            if (queue)
                m_queues[i].emplace_back(*this, queue, i, j, queue_family_properties[i].queueFlags);
        }
    }
}

DAEbool Device::Create(Instance const& in_instance, std::unique_ptr<Device>& out_device)
{
    // Uses an ordered map to automatically sort candidates by increasing score.
    std::multimap<DAEuint32, PhysicalDevice const*> candidates;

    for (auto const& physical_device : in_instance.GetPhysicalDevices())
        candidates.insert(std::make_pair(RateDeviceSuitability(physical_device), &physical_device));

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (candidate.first > 0u)
        {
            out_device = std::make_unique<Device>(*candidate.second);
            return true;
        }
    }

    GRenderer->GetLogger()->Fatal("No suitable GPU!");

    return false;
}

DAEvoid Device::WaitIdle() const noexcept
{
    vkDeviceWaitIdle(m_handle);
}

PhysicalDevice const& Device::GetPhysicalDevice() const noexcept
{
    return m_physical_device;
}

VkDevice const& Device::GetHandle() const noexcept
{
    return m_handle;
}

VmaAllocator const& Device::GetAllocator() const noexcept
{
    return m_allocator;
}

PipelineCache const& Device::GetPipelineCache() const noexcept
{
    return *m_pipeline_cache;
}

Queue const& Device::GetQueueByFlags(VkQueueFlags const in_flags) const
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if ((queue.GetFlags() & in_flags) == in_flags)
                return queue;
        }
    }

    throw std::runtime_error("Queue not found!");
}

Queue const& Device::GetPresentQueue(VkSurfaceKHR const& in_surface) const
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if (queue.IsPresentationSupported(in_surface))
                return queue;
        }
    }

    throw std::runtime_error("Queue not found!");
}

#pragma endregion