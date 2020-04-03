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

#include "Vulkan/Device.hpp"
#include "Vulkan/Semaphore.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Semaphore::Semaphore(Device const& in_device, VkSemaphoreType const in_type) noexcept:
    m_device {in_device},
    m_handle {nullptr}
{
    VkSemaphoreTypeCreateInfo semaphore_type_create_info = {};

    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    semaphore_type_create_info.semaphoreType = in_type;
    semaphore_type_create_info.initialValue  = 0ull;

    VkSemaphoreCreateInfo semaphore_create_info = {};

    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = &semaphore_type_create_info;

    vkCreateSemaphore(m_device.GetHandle(), &semaphore_create_info, nullptr, &m_handle);
}

Semaphore::~Semaphore() noexcept
{
    if (m_handle)
        vkDestroySemaphore(m_device.GetHandle(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

Device const& Semaphore::GetDevice() const noexcept
{
    return m_device;
}

VkSemaphore const& Semaphore::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion