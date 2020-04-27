﻿/*
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

#include <memory>

#include "Vulkan/Core/VulkanFence.hpp"

#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

VulkanFence::VulkanFence() noexcept
{
    VkFenceCreateInfo create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(VulkanLoader::GetLoadedDevice(), &create_info, nullptr, &m_handle);
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

DAEvoid VulkanFence::Reset() const noexcept
{
    vkResetFences(VulkanLoader::GetLoadedDevice(), 1u, &m_handle);
}

DAEvoid VulkanFence::Wait() const noexcept
{
    vkWaitForFences(VulkanLoader::GetLoadedDevice(), 1u, &m_handle, VK_TRUE, UINT64_MAX);
}

DAEbool VulkanFence::IsSignaled() const noexcept
{
    return vkGetFenceStatus(VulkanLoader::GetLoadedDevice(), m_handle) == VK_SUCCESS;
}

VkFence const& VulkanFence::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion