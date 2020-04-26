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

#include <set>

#include "Vulkan/Core/VulkanDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

#include "Threading/Scheduler.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

VulkanDevice::VulkanDevice(Scheduler const& in_scheduler, VulkanPhysicalDevice const& in_physical_device) noexcept:
    m_queue_families {in_physical_device.GetQueueFamilies()}
{
    CreateDevice      (in_physical_device);
    CreateQueues      (in_physical_device);
    CreateCommandPools(in_scheduler);

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

DAEvoid VulkanDevice::CreateDevice(VulkanPhysicalDevice const& in_physical_device) noexcept
{
    std::set<DAEuint32> const unique_queue_families = {
        m_queue_families.graphics.value(),
        m_queue_families.compute .value(),
        m_queue_families.transfer.value(),
    };

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    auto priority = 1.0f;

    for (auto const& queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info = {};

        queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount       = 1u;
        queue_create_info.pQueuePriorities = &priority;

        queue_create_infos.emplace_back(queue_create_info);
    }

    VkDeviceCreateInfo device_create_info = {};

    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount    = static_cast<DAEuint32>(queue_create_infos.size());
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.enabledExtensionCount   = static_cast<DAEuint32>(VulkanPhysicalDevice::GetRequiredExtensions().size());
    device_create_info.ppEnabledExtensionNames = VulkanPhysicalDevice::GetRequiredExtensions().data();

    VK_ASSERT(vkCreateDevice(in_physical_device.GetHandle(), &device_create_info, nullptr, &m_handle));

    VulkanLoader::LoadDevice(m_handle);
}

DAEvoid VulkanDevice::CreateQueues(VulkanPhysicalDevice const& in_physical_device) noexcept
{
    std::set<DAEuint32> const unique_queue_families = {
        m_queue_families.graphics.value(),
        m_queue_families.compute .value(),
        m_queue_families.transfer.value(),
    };

    VkQueue queue = nullptr;

    for (auto const& queue_family : unique_queue_families)
    {
        vkGetDeviceQueue(m_handle, queue_family, 0u, &queue);

        m_queues.emplace_back(in_physical_device.GetHandle(), queue, queue_family);
    }
}

DAEvoid VulkanDevice::CreateCommandPools(Scheduler const& in_scheduler) noexcept
{
    std::set<DAEuint32> const unique_queue_families = {
        m_queue_families.graphics.value(),
        m_queue_families.compute .value(),
        m_queue_families.transfer.value(),
    };

    for (auto const& queue_family : unique_queue_families)
    {
        for (auto const& worker : in_scheduler.GetWorkers())
            m_command_pools[queue_family].emplace(worker.ID(), VulkanCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));

        m_command_pools[queue_family].emplace(std::this_thread::get_id(), VulkanCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));
    }
}

DAEvoid VulkanDevice::WaitIdle() const noexcept
{
    VK_ASSERT(vkDeviceWaitIdle(m_handle));
}

VkDevice const& VulkanDevice::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 VulkanDevice::GetGraphicsFamily() const noexcept
{
    return *m_queue_families.graphics;
}

DAEuint32 VulkanDevice::GetComputeFamily() const noexcept
{
    return *m_queue_families.compute;
}

DAEuint32 VulkanDevice::GetTransferFamily() const noexcept
{
    return *m_queue_families.transfer;
}

std::optional<DAEuint32> VulkanDevice::GetPresentFamily(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue : m_queues)
    {
        if (queue.IsPresentationSupported(in_surface))
            return queue.GetQueueFamily();
    }

    return std::nullopt;
}

VulkanQueue const& VulkanDevice::GetGraphicsQueue() const noexcept
{
    return m_queues[*m_queue_families.graphics];
}

VulkanQueue const& VulkanDevice::GetComputeQueue() const noexcept
{
    return m_queues[*m_queue_families.compute];
}

VulkanQueue const& VulkanDevice::GetTransferQueue() const noexcept
{
    return m_queues[*m_queue_families.transfer];
}

VulkanQueue const* VulkanDevice::GetPresentQueue(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue : m_queues)
    {
        if (queue.IsPresentationSupported(in_surface))
            return &queue;
    }

    return nullptr;
}

VulkanCommandPool const& VulkanDevice::GetGraphicsCommandPool() const noexcept
{
    return m_command_pools.at(*m_queue_families.graphics).at(std::this_thread::get_id());
}

VulkanCommandPool const& VulkanDevice::GetComputeCommandPool() const noexcept
{
    return m_command_pools.at(*m_queue_families.compute).at(std::this_thread::get_id());
}

VulkanCommandPool const& VulkanDevice::GetTransferCommandPool() const noexcept
{
    return m_command_pools.at(*m_queue_families.transfer).at(std::this_thread::get_id());
}

VulkanPipelineCache const& VulkanDevice::GetPipelineCache() const noexcept
{
    return *m_pipeline_cache;
}

#pragma endregion