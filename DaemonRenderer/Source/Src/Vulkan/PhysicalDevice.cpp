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

#include "Vulkan/PhysicalDevice.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

PhysicalDevice::PhysicalDevice(Instance const& in_instance, VkPhysicalDevice in_handle):
    m_instance          {in_instance},
    m_handle            {in_handle},
    m_properties        {},
    m_memory_properties {},
    m_features          {}
{
    vkGetPhysicalDeviceProperties      (m_handle, &m_properties);
    vkGetPhysicalDeviceMemoryProperties(m_handle, &m_memory_properties);
    vkGetPhysicalDeviceFeatures        (m_handle, &m_features);

    DAEuint32 count = 0u;

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, nullptr);

    m_queue_family_properties.resize(count);

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, m_queue_family_properties.data());
}

#pragma endregion

#pragma region Methods

Instance const& PhysicalDevice::GetInstance() const noexcept
{
    return m_instance;
}

VkPhysicalDevice const& PhysicalDevice::GetHandle() const noexcept
{
    return m_handle;
}

VkPhysicalDeviceProperties const& PhysicalDevice::GetProperties() const noexcept
{
    return m_properties;
}

VkPhysicalDeviceMemoryProperties const& PhysicalDevice::GetMemoryProperties() const noexcept
{
    return m_memory_properties;
}

VkPhysicalDeviceFeatures const& PhysicalDevice::GetFeatures() const noexcept
{
    return m_features;
}

std::vector<VkQueueFamilyProperties> const& PhysicalDevice::GetQueueFamilyProperties() const noexcept
{
    return m_queue_family_properties;
}

#pragma endregion