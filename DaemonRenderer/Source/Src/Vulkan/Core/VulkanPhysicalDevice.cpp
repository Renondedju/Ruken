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

#include <map>
#include <set>

#include "Vulkan/Core/VulkanPhysicalDevice.hpp"
#include "Vulkan/Core/VulkanInstance.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Static Members

std::vector<DAEchar const*> VulkanPhysicalDevice::m_required_extensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructor

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance const& in_instance) noexcept
{
    PickPhysicalDevice (in_instance);
    SetupPhysicalDevice();
    SetupQueueFamilies ();
}

#pragma endregion

#pragma region Methods

DAEbool VulkanPhysicalDevice::CheckDeviceExtensions(VkPhysicalDevice in_handle) noexcept
{
    auto count = 0u;

    // Returns the number of available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_handle, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the available physical device extensions.
    vkEnumerateDeviceExtensionProperties(in_handle, nullptr, &count, supported_extensions.data());

    std::set<std::string> required_extensions(m_required_extensions.cbegin(),
                                              m_required_extensions.cend  ());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);

        if (required_extensions.empty())
            return true;
    }

    return false;
}

DAEbool VulkanPhysicalDevice::CheckQueueFamilies(VkPhysicalDevice in_handle) noexcept
{
    auto count = 0u;

    vkGetPhysicalDeviceQueueFamilyProperties(in_handle, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties(count);

    vkGetPhysicalDeviceQueueFamilyProperties(in_handle, &count, queue_family_properties.data());

    QueueFamilyIndices queue_families;

    for (auto i = 0u; i < static_cast<DAEuint32>(queue_family_properties.size()); ++i)
    {
        if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            queue_families.graphics = i;
        if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            queue_families.compute  = i;
        if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            queue_families.transfer = i;
    }

    return queue_families.IsComplete();
}

DAEuint32 VulkanPhysicalDevice::RateDeviceSuitability(VkPhysicalDevice in_handle) noexcept
{
    if (!CheckDeviceExtensions(in_handle) || !CheckQueueFamilies(in_handle))
        return 0u;

    VkPhysicalDeviceProperties       properties;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceFeatures         features;

    vkGetPhysicalDeviceProperties      (in_handle, &properties);
    vkGetPhysicalDeviceMemoryProperties(in_handle, &memory_properties);
    vkGetPhysicalDeviceFeatures        (in_handle, &features);

    if (properties.apiVersion < VK_API_VERSION_1_2)
        return 0u;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return 1000u;

    return 0u;
}

DAEvoid VulkanPhysicalDevice::PickPhysicalDevice(VulkanInstance const& in_instance) noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of physical devices accessible to this Vulkan instance.
    VK_CHECK(vkEnumeratePhysicalDevices(in_instance.GetHandle(), &count, nullptr));

    std::vector<VkPhysicalDevice> physical_devices(count);

    // Returns the physical devices accessible to this Vulkan instance.
    VK_CHECK(vkEnumeratePhysicalDevices(in_instance.GetHandle(), &count, physical_devices.data()));

    // Uses an ordered map to automatically sort candidates by increasing score.
    std::multimap<DAEuint32, VkPhysicalDevice> candidates;

    for (auto const& physical_device : physical_devices)
        candidates.insert(std::make_pair(RateDeviceSuitability(physical_device), physical_device));

    for (auto const& candidate : candidates)
    {
        if (candidate.first > 0u)
        {
            m_handle = candidate.second;
            return;
        }
    }

    VulkanDebug::GetLogger().Fatal("No suitable GPU!");

    exit(1);
}

DAEvoid VulkanPhysicalDevice::SetupPhysicalDevice() noexcept
{
    vkGetPhysicalDeviceProperties      (m_handle, &m_properties);
    vkGetPhysicalDeviceMemoryProperties(m_handle, &m_memory_properties);
    vkGetPhysicalDeviceFeatures        (m_handle, &m_features);
}

DAEvoid VulkanPhysicalDevice::SetupQueueFamilies() noexcept
{
    auto count = 0u;

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, nullptr);

    m_queue_family_properties.resize(count);

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, m_queue_family_properties.data());

    for (auto i = 0u; i < static_cast<DAEuint32>(m_queue_family_properties.size()); ++i)
    {
        if (!m_queue_families.graphics && m_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            m_queue_families.graphics = i;
        if (!m_queue_families.compute  && m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            m_queue_families.compute  = i;
        if (!m_queue_families.transfer && m_queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            m_queue_families.transfer = i;

        // Dedicated compute queue gives better performance.
        if (m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT && (m_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
            m_queue_families.compute = i;

        // Dedicated transfer queue gives better performance.
        if (m_queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT && (m_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
                                                                               (m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)  == 0)
            m_queue_families.transfer = i;
    }

    if (m_queue_families.IsComplete())
        return;

    VulkanDebug::GetLogger().Fatal("");
    exit(1);
}

std::vector<DAEchar const*> const& VulkanPhysicalDevice::GetRequiredExtensions() noexcept
{
    return m_required_extensions;
}

VkPhysicalDevice const& VulkanPhysicalDevice::GetHandle() const noexcept
{
    return m_handle;
}

VkPhysicalDeviceProperties const& VulkanPhysicalDevice::GetProperties() const noexcept
{
    return m_properties;
}

VkPhysicalDeviceMemoryProperties const& VulkanPhysicalDevice::GetMemoryProperties() const noexcept
{
    return m_memory_properties;
}

VkPhysicalDeviceFeatures const& VulkanPhysicalDevice::GetFeatures() const noexcept
{
    return m_features;
}

QueueFamilyIndices const& VulkanPhysicalDevice::GetQueueFamilies() const noexcept
{
    return m_queue_families;
}

std::vector<VkQueueFamilyProperties> const& VulkanPhysicalDevice::GetQueueFamilyProperties() const noexcept
{
    return m_queue_family_properties;
}

#pragma endregion