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

#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

VulkanDevice::VulkanDevice(VulkanPhysicalDevice const& in_physical_device)
{
    CreateDevice(in_physical_device);

    VulkanLoader::LoadDevice(m_handle);

    SetupQueues      (in_physical_device);
    SetupCommandPools(in_physical_device);

    m_pipeline_cache = std::make_unique<VulkanPipelineCache>();
}

VulkanDevice::~VulkanDevice() noexcept
{
    if (!m_handle)
        return;

    m_pipeline_cache.reset();
    m_command_pools .clear();
    m_queues        .clear();

    vkDestroyDevice(m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

DAEvoid VulkanDevice::CreateDevice(VulkanPhysicalDevice const& in_physical_device)
{
    auto const& queue_family_properties = in_physical_device.GetQueueFamilyProperties();

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
    device_create_info.enabledExtensionCount   = static_cast<DAEuint32>(VulkanPhysicalDevice::GetRequiredExtensions().size());
    device_create_info.ppEnabledExtensionNames = VulkanPhysicalDevice::GetRequiredExtensions().data();

    VK_CHECK(vkCreateDevice(in_physical_device.GetHandle(), &device_create_info, nullptr, &m_handle));
}

DAEvoid VulkanDevice::SetupQueues(VulkanPhysicalDevice const& in_physical_device)
{
    auto const& queue_family_properties = in_physical_device.GetQueueFamilyProperties();

    m_queues.resize(queue_family_properties.size());

    VkQueue queue = nullptr;

    for (auto queue_family_index = 0u; queue_family_index < static_cast<DAEuint32>(queue_family_properties.size()); ++queue_family_index)
    {
        for (auto queue_index = 0u; queue_index < queue_family_properties[queue_family_index].queueCount; ++queue_index)
        {
            vkGetDeviceQueue(m_handle, queue_family_index, queue_index, &queue);

            m_queues[queue_family_index].emplace_back(in_physical_device.GetHandle(),
                                                      queue,
                                                      queue_family_index,
                                                      queue_index,
                                                      queue_family_properties[queue_family_index].queueFlags);
        }
    }
}

DAEvoid VulkanDevice::SetupCommandPools(VulkanPhysicalDevice const& in_physical_device)
{
    auto const& queue_family_properties = in_physical_device.GetQueueFamilyProperties();

    for (auto queue_family_index = 0u; queue_family_index < static_cast<DAEuint32>(queue_family_properties.size()); ++queue_family_index)
    {
        m_command_pools[queue_family_index].emplace(std::this_thread::get_id(), VulkanCommandPool(queue_family_index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));
    }
}

std::optional<VulkanCommandBuffer> VulkanDevice::AllocateCommandBuffer(DAEuint32 const in_queue_family_index, VkCommandBufferLevel const in_level)
{
    if (in_queue_family_index >= m_command_pools.size())
        return std::nullopt;

    auto const it = m_command_pools[in_queue_family_index].find(std::this_thread::get_id());

    if (it == m_command_pools[in_queue_family_index].cend())
        return std::nullopt;

    return it->second.AllocateCommandBuffer(in_level);
}

DAEvoid VulkanDevice::WaitIdle() const noexcept
{
    VK_CHECK(vkDeviceWaitIdle(m_handle));
}

VkDevice const& VulkanDevice::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 VulkanDevice::GetGraphicsFamily() const noexcept
{
    return m_graphics_family;
}

DAEuint32 VulkanDevice::GetComputeFamily() const noexcept
{
    return m_compute_family;
}

DAEuint32 VulkanDevice::GetTransferFamily() const noexcept
{
    return m_transfer_family;
}

DAEuint32 VulkanDevice::GetPresentFamily(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue_family : m_queues)
    {
        if (queue_family.empty())
            continue;

        if (queue_family[0].IsPresentationSupported(in_surface))
                return queue_family[0].GetFamilyIndex();
    }

    return UINT32_MAX;
}

VulkanQueue* VulkanDevice::GetGraphicsQueue() noexcept
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_GRAPHICS_BIT)
                return &queue;
        }
    }

    return nullptr;
}

VulkanQueue* VulkanDevice::GetComputeQueue() noexcept
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_COMPUTE_BIT)
                return &queue;
        }
    }

    return nullptr;
}

VulkanQueue* VulkanDevice::GetTransferQueue() noexcept
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if (queue.GetFlags() & VK_QUEUE_TRANSFER_BIT)
                return &queue;
        }
    }

    return nullptr;
}

VulkanQueue* VulkanDevice::GetPresentQueue(VkSurfaceKHR const& in_surface) noexcept
{
    for (auto& queue_family : m_queues)
    {
        for (auto& queue : queue_family)
        {
            if (queue.IsPresentationSupported(in_surface))
                return &queue;
        }
    }

    return nullptr;
}

VulkanPipelineCache& VulkanDevice::GetPipelineCache() const noexcept
{
    return *m_pipeline_cache;
}

#pragma endregion