
#include "Vulkan/Core/VulkanCommandPool.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors and Destructor

VulkanCommandPool::VulkanCommandPool(RkUint32 const in_queue_family, VkCommandPoolCreateFlags const in_flags) noexcept
{
    VkCommandPoolCreateInfo command_pool_create_info = {};

    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags            = in_flags;
    command_pool_create_info.queueFamilyIndex = in_queue_family;

    VK_CHECK(vkCreateCommandPool(VulkanLoader::GetLoadedDevice(), &command_pool_create_info, nullptr, &m_handle));
}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& in_move) noexcept:
    m_handle {in_move.m_handle}
{
    in_move.m_handle = nullptr;
}

VulkanCommandPool::~VulkanCommandPool() noexcept
{
    if (!m_handle)
        return;

    vkDestroyCommandPool(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

std::optional<VulkanCommandBuffer> VulkanCommandPool::AllocateCommandBuffer(VkCommandBufferLevel const in_level) const noexcept
{
    VkCommandBuffer             command_buffer               = nullptr;
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool        = m_handle;
    command_buffer_allocate_info.level              = in_level;
    command_buffer_allocate_info.commandBufferCount = 1u;

    if (VK_CHECK(vkAllocateCommandBuffers(VulkanLoader::GetLoadedDevice(), &command_buffer_allocate_info, &command_buffer)))
        return std::nullopt;

    return VulkanCommandBuffer(command_buffer, m_handle);
}

VkCommandPool const& VulkanCommandPool::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion

#pragma region Operators

VulkanCommandPool& VulkanCommandPool::operator=(VulkanCommandPool&& in_move) noexcept
{
    m_handle = in_move.m_handle;

    in_move.m_handle = nullptr;

    return *this;
}

#pragma endregion