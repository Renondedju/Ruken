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

PhysicalDevice::PhysicalDevice(Instance* in_instance, VkPhysicalDevice in_handle) noexcept :
    m_instance  { in_instance },
    m_handle    { in_handle }
{

}

#pragma endregion

#pragma region Methods

Instance* PhysicalDevice::GetInstance() const noexcept
{
    return m_instance;
}

VkPhysicalDevice PhysicalDevice::GetHandle() const noexcept
{
    return m_handle;
}

VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const noexcept
{
    VkPhysicalDeviceProperties properties;

    vkGetPhysicalDeviceProperties(m_handle, &properties);

    return properties;
}

VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const noexcept
{
    VkPhysicalDeviceFeatures features;

    vkGetPhysicalDeviceFeatures(m_handle, &features);

    return features;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const noexcept
{
    VkPhysicalDeviceMemoryProperties memory_properties;

    vkGetPhysicalDeviceMemoryProperties(m_handle, &memory_properties);

    return memory_properties;
}

Vector<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const noexcept
{
    DAEuint32 count = 0u;

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, nullptr);

    Vector<VkQueueFamilyProperties> queue_family_properties(count);

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, queue_family_properties.data());

    return queue_family_properties;
}

#pragma endregion