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

#include "Vulkan/Device.hpp"

#include "Vulkan/Image.hpp"
#include "Vulkan/Queue.hpp"
#include "Vulkan/Buffer.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/CommandPool.hpp"
#include "Vulkan/PipelineCache.hpp"
#include "Vulkan/PhysicalDevice.hpp"

#include "Rendering/Renderer.hpp"

#include "Containers/Set.hpp"
#include "Containers/Map.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

static Vector<DAEchar const*> RequiredExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#pragma region Constructor and Destructor

Device::Device(PhysicalDevice* in_physical_device) :
    m_physical_device   { in_physical_device },
    m_handle            { nullptr },
    m_memory_allocator  { nullptr },
    m_pipeline_cache    { nullptr }
{
    if (SetupDevice())
    {
        GRenderer->GetLogger()->Info("Device created successfully.");

        volkLoadDevice(m_handle);

        if (SetupMemoryAllocator())
        {
            GRenderer->GetLogger()->Info("Device memory allocator created successfully.");

            SetupQueues();
        }

        else
            GRenderer->GetLogger()->Fatal("Failed to create device memory allocator!");
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create logical device!");
}

Device::~Device() noexcept
{
    delete m_pipeline_cache;

    for (auto const& queue_command_pool : m_command_pools)
    {
        for (auto const& command_pool : queue_command_pool.second)
        {
            delete command_pool.second;
        }
    }

    for (auto const& queue_family : m_queues)
    {
        for (auto const& queue : queue_family)
        {
            delete queue;
        }
    }

    if (m_memory_allocator)
    {
        vmaDestroyAllocator(m_memory_allocator);
    }

    if (m_handle)
    {
        vkDestroyDevice(m_handle, nullptr);
    }
}

#pragma endregion

#pragma region Methods

DAEbool Device::CheckDeviceExtensions(PhysicalDevice* in_physical_device) noexcept
{
    DAEuint32 count;

    // Returns the number available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_physical_device->GetHandle(), nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_physical_device->GetHandle(), nullptr, &count, supported_extensions.data());

    Set<String> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

DAEuint32 Device::RateDeviceSuitability(PhysicalDevice* in_physical_device) noexcept
{
    if (!CheckDeviceExtensions(in_physical_device))
        return 0u;

    VkPhysicalDeviceProperties properties;

    vkGetPhysicalDeviceProperties(in_physical_device->GetHandle(), &properties);

    if (properties.apiVersion < VK_API_VERSION_1_2)
        return 0u;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return 1000u;

    return 0u;
}

DAEbool Device::SetupDevice() noexcept
{
    auto const queue_family_properties = m_physical_device->GetQueueFamilyProperties();

    Vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_properties.size());

    Vector<Vector<DAEfloat>> priorities(queue_family_properties.size());

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

    return vkCreateDevice(m_physical_device->GetHandle(), &device_create_info, nullptr, &m_handle) == VK_SUCCESS;
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

    allocator_create_info.physicalDevice   = m_physical_device->GetHandle();
    allocator_create_info.device           = m_handle;
    allocator_create_info.pVulkanFunctions = &functions;

    return vmaCreateAllocator(&allocator_create_info, &m_memory_allocator) == VK_SUCCESS;
}

DAEvoid Device::SetupQueues()
{
    auto const queue_family_properties = m_physical_device->GetQueueFamilyProperties();

    m_queues.resize(queue_family_properties.size());

    for (DAEsize i = 0; i < queue_family_properties.size(); ++i)
    {
        for (auto j = 0u; j < queue_family_properties[i].queueCount; ++j)
        {
            VkQueue queue;

            vkGetDeviceQueue(m_handle, i, j, &queue);

            if (queue)
            {
                m_queues[i].push_back(new Queue(this, queue, i, j, queue_family_properties[i].queueFlags));
            }
        }
    }
}

DAEbool Device::Create(Instance* in_instance, Device** out_device)
{
    // Uses an ordered map to automatically sort candidates by increasing score.
    Multimap<DAEuint32, PhysicalDevice*> candidates;

    for (auto const& physical_device : in_instance->GetPhysicalDevices())
    {
        candidates.insert(std::make_pair(RateDeviceSuitability(physical_device), physical_device));
    }

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (candidate.first > 0u)
        {
            *out_device = new Device(candidate.second);

            return true;
        }
    }

    GRenderer->GetLogger()->Fatal("No suitable GPU!");

    return false;
}

DAEvoid Device::Destroy(Device* in_device)
{
    delete in_device;
}

DAEbool Device::CreateBuffer(Buffer** out_buffer)
{
    VkBuffer          buffer          = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (vmaCreateBuffer(m_memory_allocator, nullptr, nullptr, &buffer, &allocation, &allocation_info) == VK_SUCCESS)
    {
        *out_buffer = new Buffer(buffer, allocation, std::move(allocation_info));

        return true;
    }

    return  false;
}

DAEvoid Device::DestroyBuffer(Buffer* in_buffer) const noexcept
{
    vmaDestroyBuffer(m_memory_allocator, in_buffer->GetHandle(), in_buffer->GetAllocation());

    delete in_buffer;
}

DAEbool Device::CreateImage(Image** out_image)
{
    VkImage       image      = nullptr;
    VmaAllocation allocation = nullptr;

    if (vmaCreateImage(m_memory_allocator, nullptr, nullptr, &image, &allocation, nullptr) == VK_SUCCESS)
    {
        *out_image = new Image(image, allocation);

        return true;
    }

    return false;
}

DAEvoid Device::DestroyImage(Image* in_image) const noexcept
{
    vmaDestroyImage(m_memory_allocator, in_image->GetHandle(), in_image->GetAllocation());

    delete in_image;
}

DAEvoid Device::WaitIdle() const noexcept
{
    vkDeviceWaitIdle(m_handle);
}

PhysicalDevice* Device::GetPhysicalDevice() const noexcept
{
    return m_physical_device;
}

VkDevice Device::GetHandle() const noexcept
{
    return m_handle;
}

PipelineCache* Device::GetPipelineCache() const noexcept
{
    return m_pipeline_cache;
}

Vector<QueueFamily> const& Device::GetQueueFamilies() const noexcept
{
    return m_queues;
}

Queue* Device::GetQueue(DAEuint32 const in_queue_family_index, DAEuint32 const in_queue_in_index) const noexcept
{
    if (in_queue_family_index >= m_queues.size())
        return nullptr;

    if (in_queue_in_index >= m_queues[in_queue_family_index].size())
        return nullptr;

    return m_queues[in_queue_family_index][in_queue_in_index];
}

Queue* Device::GetQueueByFlags(VkQueueFlags const in_queue_flags, DAEuint32 const in_queue_in_index) const noexcept
{
    auto const queue_family_properties = m_physical_device->GetQueueFamilyProperties();

    VkQueueFlags min_flags  = ~0u;
    Queue*       best_queue = nullptr;

    for (DAEsize i = 0; i < queue_family_properties.size(); ++i)
    {
        if ((queue_family_properties[i].queueFlags & in_queue_flags) == in_queue_flags && in_queue_in_index < queue_family_properties[i].queueCount)
        {
            if (queue_family_properties[i].queueFlags < min_flags)
            {
                min_flags = queue_family_properties[i].queueFlags;

                best_queue = m_queues[i][in_queue_in_index];
            }
        }
    }

    return best_queue;
}

CommandPool* Device::GetCommandPool(Queue* in_queue)
{
    CommandPool* command_pool = nullptr;

    // TODO : Begin thread-safe block.

    auto const it = m_command_pools.find(std::this_thread::get_id());

    if (it != m_command_pools.end())
    {
        auto& queue_command_pools = it->second;

        auto const it2 = queue_command_pools.find(in_queue);

        if (it2 != queue_command_pools.end())
        {
            command_pool = it2->second;
        }

        else
        {
            queue_command_pools.emplace(in_queue, command_pool);
        }
    }

    else
    {
        QueueCommandPools queue_command_pools;

        queue_command_pools.emplace(in_queue, command_pool);

        m_command_pools.emplace(std::this_thread::get_id(), std::move(queue_command_pools));
    }

    // TODO : End thread-safe block.

    return command_pool;
}

#pragma endregion