
#include "Vulkan/Core/VulkanSemaphore.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors and Destructor

VulkanSemaphore::VulkanSemaphore() noexcept
{
    VkSemaphoreCreateInfo semaphore_create_info = {};

    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CHECK(vkCreateSemaphore(VulkanLoader::GetLoadedDevice(), &semaphore_create_info, nullptr, &m_handle));
}

VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& in_move) noexcept:
    m_handle {in_move.m_handle}
{
    in_move.m_handle = nullptr;
}

VulkanSemaphore::~VulkanSemaphore() noexcept
{
    if (!m_handle)
        return;

    vkDestroySemaphore(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

VkSemaphore const& VulkanSemaphore::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion

#pragma region Operators

VulkanSemaphore& VulkanSemaphore::operator=(VulkanSemaphore&& in_move) noexcept
{
    m_handle = in_move.m_handle;

    in_move.m_handle = nullptr;

    return *this;
}

#pragma endregion