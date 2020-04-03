/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Vulkan/Fence.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Fence::Fence(Device const& in_device) noexcept:
    m_device {in_device},
    m_handle {nullptr}
{
    VkFenceCreateInfo create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    vkCreateFence(m_device.GetHandle(), &create_info, nullptr, &m_handle);
}

Fence::~Fence() noexcept
{
    if (m_handle)
        vkDestroyFence(m_device.GetHandle(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

DAEvoid Fence::Reset() const noexcept
{
    vkResetFences(m_device.GetHandle(), 1u, &m_handle);
}

DAEvoid Fence::Wait() const noexcept
{
    vkWaitForFences(m_device.GetHandle(), 1u, &m_handle, VK_TRUE, ~0u);
}

DAEbool Fence::IsSignaled() const noexcept
{
    return vkGetFenceStatus(m_device.GetHandle(), m_handle) == VK_SUCCESS;
}

Device const& Fence::GetDevice() const noexcept
{
    return m_device;
}

VkFence const& Fence::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion