/*
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

#include "Vulkan/Core/VulkanTimelineSemaphore.hpp"

USING_DAEMON_NAMESPACE

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

    vkCreateSemaphore(VulkanLoader::GetLoadedDevice(), &semaphore_create_info, nullptr, &m_handle);
}

VulkanTimelineSemaphore::VulkanTimelineSemaphore(VulkanTimelineSemaphore&& in_move) noexcept: VulkanSemaphore(std::move(in_move))
{

}

#pragma endregion

#pragma region Methods

DAEvoid VulkanTimelineSemaphore::Signal(DAEuint64 const in_value) const noexcept
{
    VkSemaphoreSignalInfo signal_info = {};

    signal_info.sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
    signal_info.semaphore = m_handle;
    signal_info.value     = in_value;

    vkSignalSemaphore(VulkanLoader::GetLoadedDevice(), &signal_info);
}

DAEvoid VulkanTimelineSemaphore::Wait(DAEuint64 const in_value) const noexcept
{
    VkSemaphoreWaitInfo wait_info = {};

    wait_info.sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    wait_info.semaphoreCount = 1u;
    wait_info.pSemaphores    = &m_handle;
    wait_info.pValues        = &in_value;

    vkWaitSemaphores(VulkanLoader::GetLoadedDevice(), &wait_info, std::numeric_limits<DAEuint64>::max());
}

DAEuint64 VulkanTimelineSemaphore::GetValue() const noexcept
{
    DAEuint64 value;

    vkGetSemaphoreCounterValue(VulkanLoader::GetLoadedDevice(), m_handle, &value);

    return value;
}

#pragma endregion