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

#pragma region Static Variables

static std::vector<DAEchar const*> RequiredExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#pragma endregion

#pragma region Constructor

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance const& in_instance)
{
    PickPhysicalDevice(in_instance);

    vkGetPhysicalDeviceProperties      (m_handle, &m_properties);
    vkGetPhysicalDeviceMemoryProperties(m_handle, &m_memory_properties);
    vkGetPhysicalDeviceFeatures        (m_handle, &m_features);

    auto count = 0u;

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, nullptr);

    m_queue_family_properties.resize(count);

    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &count, m_queue_family_properties.data());
}

#pragma endregion

#pragma region Methods

DAEbool VulkanPhysicalDevice::CheckDeviceExtensions(VkPhysicalDevice in_handle) noexcept
{
    auto count = 0u;

    // Returns the number of available physical device extensions.
    VK_CHECK(vkEnumerateDeviceExtensionProperties(in_handle, nullptr, &count, nullptr));

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the available physical device extensions.
    VK_CHECK(vkEnumerateDeviceExtensionProperties(in_handle, nullptr, &count, supported_extensions.data()));

    std::set<std::string> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

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

    std::vector<VkQueueFamilyProperties> queue_families(count);

    vkGetPhysicalDeviceQueueFamilyProperties(in_handle, &count, queue_families.data());

    auto graphics_support = false;
    auto compute_support  = false;

    for (auto i = 0u; i < count; ++i)
    {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            graphics_support = true;
        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            compute_support = true;
        if (graphics_support && compute_support)
            return true;
    }

    return false;
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

    exit(-1);
}

std::vector<DAEchar const*> const& VulkanPhysicalDevice::GetRequiredExtensions() noexcept
{
    return RequiredExtensions;
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

std::vector<VkQueueFamilyProperties> const& VulkanPhysicalDevice::GetQueueFamilyProperties() const noexcept
{
    return m_queue_family_properties;
}

#pragma endregion