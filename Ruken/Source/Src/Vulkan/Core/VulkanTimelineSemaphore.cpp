
#include "Vulkan/Core/VulkanTimelineSemaphore.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanTimelineSemaphore::VulkanTimelineSemaphore() noexcept
{
    VkSemaphoreTypeCreateInfo semaphore_type_create_info = {};

    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    semaphore_type_create_info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphore_type_create_info.initialValue  = 0ull;

    VkSemaphoreCreateInfo semaphore_create_info = {};

    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = &semaphore_type_create_info;

    VK_CHECK(vkCreateSemaphore(VulkanLoader::GetLoadedDevice(), &semaphore_create_info, nullptr, &m_handle));
}

#pragma endregion

#pragma region Methods

RkBool VulkanTimelineSemaphore::Signal(RkUint64 const in_value) const noexcept
{
    VkSemaphoreSignalInfo signal_info = {};

    signal_info.sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
    signal_info.semaphore = m_handle;
    signal_info.value     = in_value;

    if (VK_CHECK(vkSignalSemaphore(VulkanLoader::GetLoadedDevice(), &signal_info)))
        return false;

    return true;
}

RkVoid VulkanTimelineSemaphore::Wait(RkUint64 const in_value) const noexcept
{
    VkSemaphoreWaitInfo wait_info = {};

    wait_info.sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    wait_info.semaphoreCount = 1u;
    wait_info.pSemaphores    = &m_handle;
    wait_info.pValues        = &in_value;

    VK_CHECK(vkWaitSemaphores(VulkanLoader::GetLoadedDevice(), &wait_info, UINT64_MAX));
}

RkUint64 VulkanTimelineSemaphore::GetValue() const noexcept
{
    auto value = 0ull;

    if (VK_CHECK(vkGetSemaphoreCounterValue(VulkanLoader::GetLoadedDevice(), m_handle, &value)))
        return UINT64_MAX;

    return value;
}

#pragma endregion