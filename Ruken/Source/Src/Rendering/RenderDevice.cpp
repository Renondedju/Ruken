#include <set>

#define VMA_IMPLEMENTATION

#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderContext.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Static Variables

constexpr std::array g_required_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructors

RenderDevice::RenderDevice(Logger* in_logger, RenderContext* in_context) noexcept:
    m_logger  {in_logger},
    m_context {in_context}
{
    if (!in_context->GetInstance())
        return;

    if (!PickPhysicalDevice())
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Fatal("Failed to find a suitable physical device!"))

    FindQueueFamilies();

    if (!CreateLogicalDevice())
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Fatal("Failed to create logical device!"))

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_logical_device);

    if (!CreateDeviceAllocator())
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Fatal("Failed to create device allocator!"))
}

RenderDevice::~RenderDevice() noexcept
{
    if (!m_logical_device)
        return;

    vmaDestroyAllocator(m_allocator);

    m_graphics_queue.reset();
    m_compute_queue .reset();
    m_transfer_queue.reset();

    m_logical_device.destroy();
}

#pragma endregion

#pragma region Methods

RkBool RenderDevice::PickPhysicalDevice() noexcept
{
    if (auto [result, physical_devices] = m_context->GetInstance().enumeratePhysicalDevices(); result == vk::Result::eSuccess)
    {
        for (auto const& physical_device : physical_devices)
        {
            auto [result2, extensions] = physical_device.enumerateDeviceExtensionProperties();

            if (result2 != vk::Result::eSuccess)
                continue;

            std::set<std::string> required_extensions(g_required_extensions.begin(), g_required_extensions.end());

            for (auto const& [extensionName, specVersion] : extensions)
            {
                required_extensions.erase(extensionName);
            }

            if (required_extensions.empty())
                m_physical_device = physical_device;
        }
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to enumerate physical devices : " + vk::to_string(result)))

    return m_physical_device;
}

RkVoid RenderDevice::FindQueueFamilies() noexcept
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
}

RkBool RenderDevice::CreateLogicalDevice() noexcept
{
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

    vk::PhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features = {
        .shaderSampledImageArrayNonUniformIndexing    = VK_TRUE,
        .descriptorBindingPartiallyBound              = VK_TRUE,
        .descriptorBindingVariableDescriptorCount     = VK_TRUE,
        .runtimeDescriptorArray                       = VK_TRUE
    };

    vk::PhysicalDeviceImagelessFramebufferFeatures imageless_framebuffer_features = {
        .pNext                = &descriptor_indexing_features,
        .imagelessFramebuffer = VK_TRUE
    };

    vk::PhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_features = {
        .pNext             = &imageless_framebuffer_features,
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
        .enabledExtensionCount   = static_cast<RkUint32>(g_required_extensions.size()),
        .ppEnabledExtensionNames = g_required_extensions.data()
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
    VmaVulkanFunctions functions = {
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

    VmaAllocatorCreateInfo allocator_create_info = {
        .physicalDevice   = m_physical_device,
        .device           = m_logical_device,
        .frameInUseCount  = 1U,
        .pVulkanFunctions = &functions,
        .instance         = m_context->GetInstance(),
        .vulkanApiVersion = VK_API_VERSION_1_2,
    };

    vmaCreateAllocator(&allocator_create_info, &m_allocator);

    if (vmaCreateAllocator(&allocator_create_info, &m_allocator) == VK_SUCCESS)
       return true;

    RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan device allocator"))

    return false;
}

vk::PhysicalDevice const& RenderDevice::GetPhysicalDevice() const noexcept
{
    return m_physical_device;
}

vk::Device const& RenderDevice::GetLogicalDevice() const noexcept
{
    return m_logical_device;
}

VmaAllocator const& RenderDevice::GetAllocator() const noexcept
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