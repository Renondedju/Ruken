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

#include "Rendering/Renderer.hpp"

#include "Containers/Set.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

LogicalDevice::LogicalDevice(PhysicalDevice const* in_physical_device) :
    m_handle            { nullptr },
    m_graphics_queue    { nullptr },
    m_present_queue     { nullptr }
{
    VkPhysicalDeviceFeatures device_features = in_physical_device->GetFeatures     ();
    QueueFamilyIndices       queue_families  = in_physical_device->GetQueueFamilies();

    float priority = 1.0f;

    std::set<DAEuint32> unique_queue_families = {
        queue_families.graphics_family.value(),
        queue_families.present_family .value()
    };

    Vector<VkDeviceQueueCreateInfo> queue_infos;

    for (DAEuint32 unique_queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_info;

        queue_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.pNext            = nullptr;
        queue_info.flags            = 0u;
        queue_info.queueFamilyIndex = unique_queue_family;
        queue_info.queueCount       = 1u;
        queue_info.pQueuePriorities = &priority;

        queue_infos.push_back(queue_info);
    }

    Vector<DAEchar const*> const& required_extensions = in_physical_device->GetRequiredExtensions();

    VkDeviceCreateInfo device_info;

    device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext                   = nullptr;
    device_info.flags                   = 0u;
    device_info.queueCreateInfoCount    = static_cast<DAEuint32>(queue_infos.size());
    device_info.pQueueCreateInfos       = queue_infos.data();
    device_info.enabledLayerCount       = 0u;
    device_info.ppEnabledLayerNames     = nullptr;
    device_info.enabledExtensionCount   = static_cast<DAEuint32>(required_extensions.size());
    device_info.ppEnabledExtensionNames = required_extensions.data();
    device_info.pEnabledFeatures        = &device_features;

    if (vkCreateDevice(in_physical_device->GetHandle(), &device_info, nullptr, &m_handle) == VK_SUCCESS)
    {
        GRenderer->GetLogger()->Info("Logical device created successfully.");

        vkGetDeviceQueue(m_handle, queue_families.graphics_family.value(), 0u, &m_graphics_queue);
        vkGetDeviceQueue(m_handle, queue_families.present_family .value(), 0u, &m_present_queue);
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create logical device!");
}

LogicalDevice::~LogicalDevice() noexcept
{
    vkDestroyDevice(m_handle, nullptr);

    GRenderer->GetLogger()->Info("Logical device destroyed.");
}

#pragma endregion

#pragma region Methods

VkDevice LogicalDevice::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion