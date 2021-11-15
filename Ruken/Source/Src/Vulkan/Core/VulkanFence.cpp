
#include <memory>

#include "Vulkan/Core/VulkanFence.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanFence::VulkanFence() noexcept
{
    VkFenceCreateInfo create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CHECK(vkCreateFence(VulkanLoader::GetLoadedDevice(), &create_info, nullptr, &m_handle));
}

VulkanFence::VulkanFence(VulkanFence&& in_move) noexcept:
    m_handle {in_move.m_handle}
{
    in_move.m_handle = nullptr;
}

VulkanFence::~VulkanFence() noexcept
{
    if (!m_handle)
        return;

    vkDestroyFence(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

RkBool VulkanFence::Reset() const noexcept
{
    if (VK_CHECK(vkResetFences(VulkanLoader::GetLoadedDevice(), 1u, &m_handle)))
        return false;

    return true;
}

RkVoid VulkanFence::Wait() const noexcept
{
    VK_CHECK(vkWaitForFences(VulkanLoader::GetLoadedDevice(), 1u, &m_handle, VK_TRUE, UINT64_MAX));
}

RkBool VulkanFence::IsSignaled() const noexcept
{
    return vkGetFenceStatus(VulkanLoader::GetLoadedDevice(), m_handle) == VK_SUCCESS;
}

VkFence const& VulkanFence::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion

#pragma region Operators

VulkanFence& VulkanFence::operator=(VulkanFence&& in_move) noexcept
{
    m_handle = in_move.m_handle;

    in_move.m_handle = nullptr;

    return *this;
}

#pragma endregion