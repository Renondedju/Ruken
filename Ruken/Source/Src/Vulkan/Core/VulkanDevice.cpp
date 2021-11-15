
#include <set>

#include "Vulkan/Core/VulkanDevice.hpp"

#include "Threading/Scheduler.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanDevice::VulkanDevice(Scheduler            const& in_scheduler,
                           VulkanPhysicalDevice const& in_physical_device) noexcept:
    m_queue_families {in_physical_device.GetQueueFamilies()}
{
    if (!CreateDevice(in_physical_device))
        return;

    CreateQueues      (in_physical_device);
    CreateCommandPools(in_scheduler);

    m_pipeline_cache = std::make_unique<VulkanPipelineCache>("Data/PipelineCache.data");
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

RkBool VulkanDevice::CreateDevice(VulkanPhysicalDevice const& in_physical_device) noexcept
{
    std::set<RkUint32> const unique_queue_families = {
        *m_queue_families.graphics,
        *m_queue_families.compute,
        *m_queue_families.transfer,
    };

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(unique_queue_families.size());

    auto priority = 1.0f;

    for (auto const& queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info = {};

        queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount       = 1u;
        queue_create_info.pQueuePriorities = &priority;

        queue_create_infos[queue_family] = std::move(queue_create_info);
    }

    VkPhysicalDeviceVulkan12Features vulkan_12_features = {};

    vulkan_12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

    VkPhysicalDeviceVulkan11Features vulkan_11_features = {};

    vulkan_11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    vulkan_11_features.pNext = &vulkan_12_features;

    VkPhysicalDeviceFeatures2 features = {};

    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features.pNext = &vulkan_11_features;

    VkDeviceCreateInfo device_create_info = {};

    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext                   = &features;
    device_create_info.queueCreateInfoCount    = static_cast<RkUint32>(queue_create_infos.size());
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.enabledExtensionCount   = static_cast<RkUint32>(VulkanPhysicalDevice::GetRequiredExtensions().size());
    device_create_info.ppEnabledExtensionNames = VulkanPhysicalDevice::GetRequiredExtensions().data();

    if (VK_ASSERT(vkCreateDevice(in_physical_device.GetHandle(), &device_create_info, nullptr, &m_handle)))
        return false;

    VulkanLoader::LoadDevice(m_handle);

    return true;
}

RkVoid VulkanDevice::CreateQueues(VulkanPhysicalDevice const& in_physical_device) noexcept
{
    std::set<RkUint32> const unique_queue_families = {
        *m_queue_families.graphics,
        *m_queue_families.compute,
        *m_queue_families.transfer,
    };

    VkQueue queue = nullptr;

    for (auto const& queue_family : unique_queue_families)
    {
        vkGetDeviceQueue(m_handle, queue_family, 0u, &queue);

        m_queues.emplace_back(in_physical_device.GetHandle(), queue, queue_family);
    }
}

RkVoid VulkanDevice::CreateCommandPools(Scheduler const& in_scheduler) noexcept
{
    std::set<RkUint32> const unique_queue_families = {
        *m_queue_families.graphics,
        *m_queue_families.compute,
        *m_queue_families.transfer,
    };

    for (auto const& queue_family : unique_queue_families)
    {
        for (auto const& worker : in_scheduler.GetWorkers())
            m_command_pools[queue_family].emplace(worker.ID(), VulkanCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));

        m_command_pools[queue_family].emplace(std::this_thread::get_id(), VulkanCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));
    }
}

RkVoid VulkanDevice::WaitIdle() const noexcept
{
    VK_CHECK(vkDeviceWaitIdle(m_handle));
}

std::optional<RkUint32> VulkanDevice::FindPresentFamily(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue : m_queues)
    {
        if (queue.IsPresentationSupported(in_surface))
            return queue.GetQueueFamily();
    }

    return std::nullopt;
}

VulkanQueue const* VulkanDevice::RequestPresentQueue(VkSurfaceKHR const& in_surface) const noexcept
{
    for (auto const& queue : m_queues)
    {
        if (queue.IsPresentationSupported(in_surface))
            return &queue;
    }

    return nullptr;
}

RkBool VulkanDevice::IsValid() const noexcept
{
    return m_handle != nullptr;
}

RkUint32 VulkanDevice::GetGraphicsFamily() const noexcept
{
    return *m_queue_families.graphics;
}

RkUint32 VulkanDevice::GetComputeFamily() const noexcept
{
    return *m_queue_families.compute;
}

RkUint32 VulkanDevice::GetTransferFamily() const noexcept
{
    return *m_queue_families.transfer;
}

VkDevice const& VulkanDevice::GetHandle() const noexcept
{
    return m_handle;
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