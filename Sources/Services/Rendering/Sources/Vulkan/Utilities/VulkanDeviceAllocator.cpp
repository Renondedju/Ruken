
#pragma warning (push, 0)

#define VMA_IMPLEMENTATION

#include "Rendering/Vulkan/Utilities/VulkanDeviceAllocator.hpp"

#pragma warning (pop)

#include "Rendering/Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Rendering/Vulkan/Utilities/VulkanDebug.hpp"
#include "Rendering/Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanDeviceAllocator::VulkanDeviceAllocator(VulkanPhysicalDevice const& in_physical_device) noexcept
{
    const VmaVulkanFunctions functions = {
        .vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties,
        .vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties,
        .vkAllocateMemory = vkAllocateMemory,
        .vkFreeMemory = vkFreeMemory,
        .vkMapMemory = vkMapMemory,
        .vkUnmapMemory = vkUnmapMemory,
        .vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges,
        .vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges,
        .vkBindBufferMemory = vkBindBufferMemory,
        .vkBindImageMemory = vkBindImageMemory,
        .vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements,
        .vkGetImageMemoryRequirements = vkGetImageMemoryRequirements,
        .vkCreateBuffer = vkCreateBuffer,
        .vkDestroyBuffer = vkDestroyBuffer,
        .vkCreateImage = vkCreateImage,
        .vkDestroyImage = vkDestroyImage,
        .vkCmdCopyBuffer = vkCmdCopyBuffer,
        .vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2,
        .vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2,
        .vkBindBufferMemory2KHR = vkBindBufferMemory2,
        .vkBindImageMemory2KHR = vkBindImageMemory2,
        .vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2
    };
    VmaAllocatorCreateInfo allocator_create_info = {};

    allocator_create_info.physicalDevice   = in_physical_device.GetHandle();
    allocator_create_info.device           = VulkanLoader::GetLoadedDevice();
    allocator_create_info.pVulkanFunctions = &functions;
    allocator_create_info.instance         = VulkanLoader::GetLoadedInstance();
    allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_2;

    VK_ASSERT(vmaCreateAllocator(&allocator_create_info, &m_handle));
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

    if (VK_CHECK(vmaCreateImage(m_handle,
                                &in_image_create_info,
                                &in_allocation_create_info,
                                &image,
                                &allocation,
                                &allocation_info)))
        return std::nullopt;

    return VulkanImage(image,
                       m_handle,
                       allocation,
                       in_image_create_info.format,
                       in_image_create_info.extent);
}

std::optional<VulkanBuffer> VulkanDeviceAllocator::CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                                                                VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept
{
    VkBuffer          buffer          = nullptr;
    VmaAllocation     allocation      = nullptr;
    VmaAllocationInfo allocation_info = {};

    if (VK_CHECK(vmaCreateBuffer(m_handle,
                                 &in_buffer_create_info,
                                 &in_allocation_create_info,
                                 &buffer,
                                 &allocation,
                                 &allocation_info)))
        return std::nullopt;

    return VulkanBuffer(buffer,
                        m_handle,
                        allocation,
                        allocation_info,
                        in_allocation_create_info.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT);
}

RkBool VulkanDeviceAllocator::IsValid() const noexcept
{
    return m_handle != nullptr;
}

#pragma endregion