#define VMA_IMPLEMENTATION

#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderContext.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Static Variables

constexpr std::array g_enabled_extensions = {
    VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME,
    VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructors

RenderDevice::RenderDevice(Logger* in_logger, RenderContext* in_context) noexcept:
    m_logger  {in_logger},
    m_context {in_context}
{
    if (!in_context->GetInstance() || !PickPhysicalDevice() || !CreateLogicalDevice())
        return;

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_logical_device);

    CreateDeviceAllocator();
}

RenderDevice::~RenderDevice() noexcept
{
    if (!m_logical_device)
        return;

    if (m_allocator)
        m_allocator.destroy();

    m_graphics_queue.reset();
    m_compute_queue .reset();
    m_transfer_queue.reset();

    m_logical_device.destroy();
}

#pragma endregion

#pragma region Methods

RkBool RenderDevice::PickPhysicalDevice() noexcept
{
    auto [result, value] = m_context->GetInstance().enumeratePhysicalDevices();

    if (result == vk::Result::eSuccess)
    {
        // TODO : Select best physical device here.
        m_physical_device = value[0];
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("No suitable physical device available : " + vk::to_string(result)))

    return result == vk::Result::eSuccess;
}

RkBool RenderDevice::CreateLogicalDevice() noexcept
{
    auto const queue_families = m_physical_device.getQueueFamilyProperties();

    for (RkUint32 index = 0U; index < queue_families.size(); ++index)
    {
        vk::QueueFlags flags = queue_families[index].queueFlags;

        if (flags & vk::QueueFlagBits::eTransfer && !(flags & vk::QueueFlagBits::eGraphics) && !(flags & vk::QueueFlagBits::eCompute))
        {
            m_transfer_family_index = index;
        }

        if (flags & vk::QueueFlagBits::eCompute && !(flags & vk::QueueFlagBits::eGraphics))
        {
            m_compute_family_index = index;
        }

        if (flags & vk::QueueFlagBits::eGraphics)
        {
            m_graphics_family_index = index;
        }
    }

    if (m_compute_family_index  == UINT32_MAX)
        m_compute_family_index  = m_graphics_family_index;
    if (m_transfer_family_index == UINT32_MAX)
        m_transfer_family_index = m_graphics_family_index;

    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

    RkFloat queue_priority = 1.0F;

    vk::DeviceQueueCreateInfo graphics_queue_create_info = {
        .queueFamilyIndex = m_graphics_family_index,
        .queueCount       = 1U,
        .pQueuePriorities = &queue_priority
    };

    queue_create_infos.emplace_back(graphics_queue_create_info);

    if (HasDedicatedComputeQueue())
    {
        vk::DeviceQueueCreateInfo compute_queue_create_info = {
            .queueFamilyIndex = m_compute_family_index,
            .queueCount       = 1U,
            .pQueuePriorities = &queue_priority
        };

        queue_create_infos.emplace_back(compute_queue_create_info);
    }

    if (HasDedicatedTransferQueue())
    {
        vk::DeviceQueueCreateInfo transfer_queue_create_info = {
            .queueFamilyIndex = m_transfer_family_index,
            .queueCount       = 1U,
            .pQueuePriorities = &queue_priority
        };

        queue_create_infos.emplace_back(transfer_queue_create_info);
    }

    vk::PhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_features = {
        .timelineSemaphore = VK_TRUE
    };

    vk::PhysicalDeviceSynchronization2FeaturesKHR synchronization_features = {
        .pNext            = &timeline_semaphore_features,
        .synchronization2 = VK_TRUE
    };

    auto features = m_physical_device.getFeatures2();

    features.pNext = &synchronization_features;

    vk::DeviceCreateInfo device_create_info = {
        .pNext                   = &features,
        .queueCreateInfoCount    = static_cast<RkUint32>(queue_create_infos.size()),
        .pQueueCreateInfos       = queue_create_infos.data(),
        .enabledExtensionCount   = static_cast<RkUint32>(g_enabled_extensions.size()),
        .ppEnabledExtensionNames = g_enabled_extensions.data()
    };

    auto [result, value] = m_physical_device.createDevice(device_create_info);

    if (result == vk::Result::eSuccess)
    {
        m_logical_device = value;

        m_graphics_queue = std::make_unique<RenderQueue>(m_logger, this, m_graphics_family_index, 0U);

        if (HasDedicatedComputeQueue())
            m_compute_queue = std::make_unique<RenderQueue>(m_logger, this, m_compute_family_index, 0U);

        if (HasDedicatedTransferQueue())
            m_transfer_queue = std::make_unique<RenderQueue>(m_logger, this, m_transfer_family_index, 0U);
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan device : " + vk::to_string(result)))

    return result == vk::Result::eSuccess;
}

RkBool RenderDevice::CreateDeviceAllocator() noexcept
{
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
        .instance         = m_context->GetInstance(),
        .vulkanApiVersion = VK_API_VERSION_1_1
    };

    auto [result, value] = createAllocator(allocator_create_info);

    if (result == vk::Result::eSuccess)
        m_allocator = value;
    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan device allocator : " + vk::to_string(result)))

    return result == vk::Result::eSuccess;
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

RkUint32 RenderDevice::GetGraphicsFamilyIndex() const noexcept
{
    return m_graphics_family_index;
}

RkUint32 RenderDevice::GetComputeFamilyIndex() const noexcept
{
    return m_compute_family_index;
}

RkUint32 RenderDevice::GetTransferFamilyIndex() const noexcept
{
    return m_transfer_family_index;
}

RenderQueue& RenderDevice::GetGraphicsQueue() const noexcept
{
    return *m_graphics_queue;
}

RenderQueue& RenderDevice::GetComputeQueue() const noexcept
{
    return HasDedicatedComputeQueue() ? *m_compute_queue : *m_graphics_queue;
}

RenderQueue& RenderDevice::GetTransferQueue() const noexcept
{
    return HasDedicatedTransferQueue() ? *m_transfer_queue : *m_graphics_queue;
}

RkBool RenderDevice::HasDedicatedComputeQueue() const noexcept
{
    return m_compute_family_index != m_graphics_family_index;
}

RkBool RenderDevice::HasDedicatedTransferQueue() const noexcept
{
    return m_transfer_family_index != m_graphics_family_index;
}

#pragma endregion