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

#include "Containers/Map.hpp"
#include "Containers/Set.hpp"

#include "Rendering/Renderer.hpp"

#include "Rendering/Utilities/Debug.hpp"

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::Renderer* DAEMON_NAMESPACE::GRenderer = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

#ifdef DAEMON_OS_WINDOWS

#include "Utility/WindowsOS.hpp"

#include <Vulkan/vulkan_win32.h>

#endif

USING_DAEMON_NAMESPACE

#pragma region Constructor

Renderer::Renderer() :
    m_logger            { GRootLogger->AddChild("Rendering") },
    m_instance          { nullptr },
    m_physical_device   { nullptr },
    m_logical_device    { nullptr }
{
    m_required_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

    #ifdef DAEMON_OS_WINDOWS

    m_required_instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    #endif

    #ifdef DAEMON_DEBUG

    m_required_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    #endif

    CheckInstanceExtensions();

    #ifdef DAEMON_DEBUG

    m_required_validation_layers.push_back("VK_LAYER_KHRONOS_validation");

    CheckValidationLayers();

    #endif

    CreateInstance();

    #ifdef DAEMON_DEBUG

    Debug::SetupDebugMessenger(m_instance,
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                               VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                               this);

    #endif

    PickPhysicalDevice();
    CreateLogicalDevice();

    m_logger->Info("Renderer successfully initialized.");
}

Renderer::~Renderer() noexcept
{
    vkDestroyDevice(m_logical_device, nullptr);

    #ifdef DAEMON_DEBUG

    Debug::FreeDebugMessenger(m_instance);

    #endif

    vkDestroyInstance(m_instance, nullptr);

    m_logger->Info("Renderer successfully shut down.");
}

#pragma endregion

#pragma region Methods

DAEvoid Renderer::CreateInstance() noexcept
{
    VkApplicationInfo app_info;

    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext              = nullptr;
    app_info.pApplicationName   = DAEMON_PROJECT_NAME;
    app_info.applicationVersion = 0u;
    app_info.pEngineName        = DAEMON_PROJECT_NAME;
    app_info.engineVersion      = 0u;
    app_info.apiVersion         = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instance_info;

    instance_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pNext                   = nullptr;
    instance_info.flags                   = 0;
    instance_info.pApplicationInfo        = &app_info;
    instance_info.enabledLayerCount       = static_cast<DAEuint32>(m_required_validation_layers.size());
    instance_info.ppEnabledLayerNames     = m_required_validation_layers.data();
    instance_info.enabledExtensionCount   = static_cast<DAEuint32>(m_required_instance_extensions.size());
    instance_info.ppEnabledExtensionNames = m_required_instance_extensions.data();

    if (vkCreateInstance(&instance_info, nullptr, &m_instance) == VK_SUCCESS)
    {
        m_logger->Info("Instance created successfully.");
    }

    else
        m_logger->Fatal("Failed to create instance!");
}

DAEvoid Renderer::PickPhysicalDevice() noexcept
{
    DAEuint32 device_count = 0u;

    // Enumerates the physical devices accessible to an instance.
    vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

    if (device_count == 0u)
    {
        m_logger->Fatal("No GPU available");
    }

    std::vector<VkPhysicalDevice> devices(device_count);

    vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

    // Uses an ordered map to automatically sort candidates by increasing score.
    Multimap<DAEuint32, VkPhysicalDevice> candidates;

    for (auto const& device : devices)
        candidates.insert(std::make_pair(RateDeviceSuitability(device), device));

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (CheckDeviceExtensions(candidate.second))
        {
            m_physical_device = candidate.second;
            break;
        }
    }

    if (m_physical_device == nullptr)
    {
        m_logger->Fatal("Failed to find a suitable GPU");
    }
}

DAEvoid Renderer::CreateLogicalDevice() noexcept
{
    VkDeviceCreateInfo device_info;

    device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext                   = nullptr;
    device_info.flags                   = 0u;
    device_info.queueCreateInfoCount    = 0u;
    device_info.pQueueCreateInfos       = nullptr;
    device_info.enabledLayerCount       = 0u;
    device_info.ppEnabledLayerNames     = nullptr;
    device_info.enabledExtensionCount   = 0u;
    device_info.ppEnabledExtensionNames = nullptr;
    device_info.pEnabledFeatures        = nullptr;

    if (vkCreateDevice(m_physical_device, &device_info, nullptr, &m_logical_device) == VK_SUCCESS)
    {
        m_logger->Info("Device created successfully.");
    }

    else
        m_logger->Fatal("Failed to create device!");
}

DAEvoid Renderer::CheckInstanceExtensions() const noexcept
{
    // Returns up to requested number of global extension properties.
    DAEuint32 extension_count = 0u;

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(extension_count);

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, supported_extensions.data());

    // Checks if all the required extensions are supported.
    std::set<String> required_extensions(m_required_instance_extensions.cbegin(), m_required_instance_extensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
        required_extensions.erase(extension.extensionName);

    // In case some extensions could not be found, enumerates them.
    if (!required_extensions.empty())
    {
        for (auto const& extension : required_extensions)
        {
            m_logger->Error("Missing instance extension : " + extension);
        }

        m_logger->Fatal("One or more required instance extensions could not be found!");
    }
}

DAEvoid Renderer::CheckValidationLayers() const noexcept
{
    // Returns up to requested number of global layer properties
    DAEuint32 layer_count = 0u;

    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> supported_layers(layer_count);

    vkEnumerateInstanceLayerProperties(&layer_count, supported_layers.data());

    // Checks if all the required layers are supported.
    std::set<String> required_layers(m_required_validation_layers.cbegin(), m_required_validation_layers.cend());

    // Removes the matching layers.
    for (auto const& layer : supported_layers)
        required_layers.erase(layer.layerName);

    // In case some layers could not be found, enumerates them.
    if (!required_layers.empty())
    {
        for (auto const& layer : required_layers)
        {
            m_logger->Error("Missing validation layer : " + layer);
        }

        m_logger->Fatal("One or more required validation layer could not be found!");
    }
}

DAEbool Renderer::CheckDeviceExtensions(VkPhysicalDevice in_physical_device) const noexcept
{
    // Returns up to requested number of global layer properties
    DAEuint32 extension_count = 0u;

    vkEnumerateDeviceExtensionProperties(in_physical_device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(extension_count);

    vkEnumerateDeviceExtensionProperties(in_physical_device, nullptr, &extension_count, supported_extensions.data());

    // Checks if all the required layers are supported.
    std::set<String> required_extensions(m_required_device_extensions.cbegin(), m_required_device_extensions.cend());

    // Removes the matching layers.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

DAEuint32 Renderer::RateDeviceSuitability(VkPhysicalDevice in_physical_device) const noexcept
{
    return 0u;
}

Logger const* Renderer::GetLogger() const noexcept
{
    return m_logger;
}

#pragma endregion