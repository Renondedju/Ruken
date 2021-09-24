#define VMA_IMPLEMENTATION

#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderContext.hpp"

USING_RUKEN_NAMESPACE

constexpr std::array g_enabled_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

RenderDevice::RenderDevice(RenderContext const& in_context, Logger* in_logger) noexcept: m_logger {in_logger}
{
    auto [result, physical_devices] = in_context.GetInstance().enumeratePhysicalDevices();

    m_physical_device = physical_devices[0];

    auto const queue_families = m_physical_device.getQueueFamilyProperties();

    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos(queue_families.size());

    std::vector<std::vector<RkFloat>> queue_priorities(queue_families.size());

    for (RkUint32 index = 0U; index < queue_families.size(); ++index)
    {
       queue_priorities[index].resize(queue_families[index].queueCount, 1.0F);

        queue_create_infos[index].queueFamilyIndex = index;
        queue_create_infos[index].queueCount       = queue_families[index].queueCount;
        queue_create_infos[index].pQueuePriorities = queue_priorities[index].data();
    }

    vk::DeviceCreateInfo device_create_info = {
        .queueCreateInfoCount    = static_cast<RkUint32>(queue_create_infos.size()),
        .pQueueCreateInfos       = queue_create_infos.data(),
        .enabledExtensionCount   = static_cast<RkUint32>(g_enabled_extensions.size()),
        .ppEnabledExtensionNames = g_enabled_extensions.data()
    };

    std::tie(result, m_logical_device) = m_physical_device.createDevice(device_create_info);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_logical_device);
     
    m_queues.resize(queue_families.size());

    for (RkUint32 queue_family_index = 0U; queue_family_index < queue_families.size(); ++queue_family_index)
    {
        m_queues[queue_family_index].resize(queue_families[queue_family_index].queueCount);

        for (RkUint32 queue_index = 0U; queue_index < queue_families[queue_family_index].queueCount; ++queue_index)
        {
            m_queues[queue_family_index][queue_index] = m_logical_device.getQueue(queue_family_index, queue_index);
        }
    }

    vk::VulkanFunctions functions = {
        .vkGetPhysicalDeviceProperties           = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceProperties,
        .vkGetPhysicalDeviceMemoryProperties     = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties,
        .vkAllocateMemory                        = VULKAN_HPP_DEFAULT_DISPATCHER.vkAllocateMemory,
        .vkFreeMemory                            = VULKAN_HPP_DEFAULT_DISPATCHER.vkFreeMemory,
        .vkMapMemory                             = VULKAN_HPP_DEFAULT_DISPATCHER.vkMapMemory,
        .vkUnmapMemory                           = VULKAN_HPP_DEFAULT_DISPATCHER.vkUnmapMemory,
        .vkFlushMappedMemoryRanges               = VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges,
        .vkInvalidateMappedMemoryRanges          = VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges,
        .vkBindBufferMemory                      = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory,
        .vkBindImageMemory                       = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory,
        .vkGetBufferMemoryRequirements           = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements,
        .vkGetImageMemoryRequirements            = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements,
        .vkCreateBuffer                          = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateBuffer,
        .vkDestroyBuffer                         = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyBuffer,
        .vkCreateImage                           = VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateImage,
        .vkDestroyImage                          = VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyImage,
        .vkCmdCopyBuffer                         = VULKAN_HPP_DEFAULT_DISPATCHER.vkCmdCopyBuffer,
        .vkGetBufferMemoryRequirements2KHR       = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements2,
        .vkGetImageMemoryRequirements2KHR        = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements2,
        .vkBindBufferMemory2KHR                  = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory2,
        .vkBindImageMemory2KHR                   = VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory2,
        .vkGetPhysicalDeviceMemoryProperties2KHR = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties2
    };

    vk::AllocatorCreateInfo allocator_create_info = {
        .physicalDevice   = m_physical_device,
        .device           = m_logical_device,
        .pVulkanFunctions = &functions,
        .instance         = in_context.GetInstance(),
        .vulkanApiVersion = VK_API_VERSION_1_1
    };

    std::tie(result, m_allocator) = createAllocator(allocator_create_info);
}

RenderDevice::~RenderDevice() noexcept
{
    m_allocator     .destroy();
    m_logical_device.destroy();
}

vk::PhysicalDevice const& RenderDevice::GetPhysicalDevice() const noexcept
{
    return m_physical_device;
}

vk::Device const& RenderDevice::GetLogicalDevice() const noexcept
{
    return m_logical_device;
}

vk::Allocator const& RenderDevice::GetAllocator() const noexcept
{
    return m_allocator;
}

vk::Queue const& RenderDevice::GetTestQueue() const noexcept
{
    return m_queues[0][0];
}
