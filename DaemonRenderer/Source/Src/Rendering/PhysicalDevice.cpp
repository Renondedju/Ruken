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
#include "Containers/Map.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

PhysicalDevice::PhysicalDevice(Instance const* in_instance,
                               Surface  const* in_surface) :
    m_surface       { in_surface },
    m_handle        { nullptr },
    m_properties    {},
    m_features      {}
{
    m_required_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    if (PickPhysicalDevice(in_instance->GetHandle()))
    {
        vkGetPhysicalDeviceProperties(m_handle, &m_properties);
        vkGetPhysicalDeviceFeatures  (m_handle, &m_features);

        m_queue_families    = FindQueueFamilies    (m_handle);
        m_surface_details = QuerySwapchainDetails(m_handle);

        GRenderer->GetLogger()->Info(String("Suitable GPU found : ") + m_properties.deviceName);
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to find a suitable GPU!");
}

#pragma endregion

#pragma region Methods

DAEbool PhysicalDevice::CheckDeviceExtensions(VkPhysicalDevice in_physical_device) const noexcept
{
    std::set<String> required_extensions(m_required_extensions.cbegin(), m_required_extensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : GetSupportedExtensions(in_physical_device))
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

DAEbool PhysicalDevice::CheckDeviceLayers(VkPhysicalDevice in_physical_device) const noexcept
{
    std::set<String> required_layers(m_required_layers.cbegin(), m_required_layers.cend());

    // Removes the matching layers.
    for (auto const& layer : GetSupportedLayers(in_physical_device))
    {
        required_layers.erase(layer.layerName);
    }

    return required_layers.empty();
}

DAEuint32 PhysicalDevice::RateDeviceSuitability(VkPhysicalDevice in_physical_device) const noexcept
{
    QueueFamilyIndices const indices = FindQueueFamilies    (in_physical_device);
    SurfaceDetails   const details = QuerySwapchainDetails(in_physical_device);

    if (!indices.graphics_family.has_value() || !indices.present_family.has_value())
        return 0u;

    if (details.formats.empty() || details.present_modes.empty())
        return 0u;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures   features;

    vkGetPhysicalDeviceProperties(in_physical_device, &properties);
    vkGetPhysicalDeviceFeatures  (in_physical_device, &features);

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return 1000u;

    return 0u;
}

QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice in_physical_device) const noexcept
{
    QueueFamilyIndices indices;
    DAEuint32          count;

    vkGetPhysicalDeviceQueueFamilyProperties(in_physical_device, &count, nullptr);

    Vector<VkQueueFamilyProperties> queue_families(count);

    vkGetPhysicalDeviceQueueFamilyProperties(in_physical_device, &count, queue_families.data());

    VkBool32 present_support = false;

    for (DAEuint32 i = 0u; i < count; ++i)
    {
        if (!indices.present_family.has_value() && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        vkGetPhysicalDeviceSurfaceSupportKHR(in_physical_device, i, m_surface->GetHandle(), &present_support);

        if (!indices.present_family.has_value() && present_support)
        {
            indices.present_family = i;
        }
    }

    return indices;
}

SurfaceDetails PhysicalDevice::QuerySwapchainDetails(VkPhysicalDevice in_physical_device) const
{
    DAEuint32        count;
    SurfaceDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(in_physical_device, m_surface->GetHandle(), &details.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(in_physical_device, m_surface->GetHandle(), &count, nullptr);

    details.formats.resize(count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(in_physical_device, m_surface->GetHandle(), &count, details.formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(in_physical_device, m_surface->GetHandle(), &count, nullptr);

    details.present_modes.resize(count);

    vkGetPhysicalDeviceSurfacePresentModesKHR(in_physical_device, m_surface->GetHandle(), &count, details.present_modes.data());

    return details;
}

DAEbool PhysicalDevice::PickPhysicalDevice(VkInstance in_instance) noexcept
{
    DAEuint32 count;

    vkEnumeratePhysicalDevices(in_instance, &count, nullptr);

    if (count == 0u)
    {
        GRenderer->GetLogger()->Fatal("No GPU available");
    }

    std::vector<VkPhysicalDevice> devices(count);

    vkEnumeratePhysicalDevices(in_instance, &count, devices.data());

    // Uses an ordered map to automatically sort candidates by increasing score.
    Multimap<DAEuint32, VkPhysicalDevice> candidates;

    for (auto const& device : devices)
    {
        candidates.insert(std::make_pair(RateDeviceSuitability(device), device));
    }

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (candidate.first > 0u && CheckDeviceExtensions(candidate.second) &&
                                    CheckDeviceLayers    (candidate.second))
        {
            m_handle = candidate.second;
            break;
        }
    }

    return m_handle != nullptr;
}

Vector<VkExtensionProperties> PhysicalDevice::GetSupportedExtensions(VkPhysicalDevice in_physical_device, DAEchar const* in_layer_name) noexcept
{
    DAEuint32 count;

    vkEnumerateDeviceExtensionProperties(in_physical_device, in_layer_name, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    vkEnumerateDeviceExtensionProperties(in_physical_device, in_layer_name, &count, supported_extensions.data());

    return supported_extensions;
}

Vector<VkLayerProperties> PhysicalDevice::GetSupportedLayers(VkPhysicalDevice in_physical_device) noexcept
{
    DAEuint32 count;

    vkEnumerateDeviceLayerProperties(in_physical_device, &count, nullptr);

    std::vector<VkLayerProperties> supported_layers(count);

    vkEnumerateDeviceLayerProperties(in_physical_device, &count, supported_layers.data());

    return supported_layers;
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

QueueFamilyIndices PhysicalDevice::GetQueueFamilies() const noexcept
{
    return m_queue_families;
}

SurfaceDetails PhysicalDevice::GetSurfaceDetails() const noexcept
{
    return m_surface_details;
}

Vector<DAEchar const*> const& PhysicalDevice::GetRequiredExtensions() const noexcept
{
    return m_required_extensions;
}

Vector<DAEchar const*> const& PhysicalDevice::GetRequiredLayers() const noexcept
{
    return m_required_layers;
}

#pragma endregion