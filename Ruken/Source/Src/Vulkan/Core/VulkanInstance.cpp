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

#include <set>
#include <vector>

#include "Vulkan/Core/VulkanInstance.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Static Members

std::vector<RkChar const*> VulkanInstance::m_required_extensions =
{
    #ifdef RUKEN_OS_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif

    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

std::vector<RkChar const*> VulkanInstance::m_required_validation_layers =
{
    #ifdef RUKEN_CONFIG_DEBUG

    "VK_LAYER_KHRONOS_validation"

    #endif
};

std::vector<VkValidationFeatureEnableEXT> VulkanInstance::m_enabled_validation_features =
{
    VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT
};

std::vector<VkValidationFeatureDisableEXT> VulkanInstance::m_disabled_validation_features =
{

};

#pragma endregion

#pragma region Constructors

VulkanInstance::VulkanInstance() noexcept
{
    if (CheckInstanceExtensions() &&
        CheckValidationLayers  () &&
        CreateInstance         ())
        VulkanDebug::CreateDebugMessenger();
}

VulkanInstance::~VulkanInstance() noexcept
{
    if (!m_handle)
        return;

    VulkanDebug::DestroyDebugMessenger();

    vkDestroyInstance(m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

RkBool VulkanInstance::CheckInstanceExtensions() noexcept
{
    auto count = 0u;

    // Returns the number of global extension properties.
    if (VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr)))
        return false;

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the global extension properties.
    if (VK_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &count, supported_extensions.data())))
        return false;

    std::set<std::string> required_extensions(m_required_extensions.cbegin(),
                                              m_required_extensions.cend  ());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);

        if (required_extensions.empty())
            return true;
    }

    // In case some extensions could not be found, enumerates them.
    for (auto const& extension : required_extensions)
        VulkanDebug::Fatal("Missing instance extension : " + extension + "!");

    return false;
}

RkBool VulkanInstance::CheckValidationLayers() noexcept
{
    auto count = 0u;

    // Returns the number of layer properties available.
    if (VK_ASSERT(vkEnumerateInstanceLayerProperties(&count, nullptr)))
        return false;

    std::vector<VkLayerProperties> supported_layers(count);

    // Returns the global layer properties
    if (VK_ASSERT(vkEnumerateInstanceLayerProperties(&count, supported_layers.data())))
        return false;

    std::set<std::string> required_layers(m_required_validation_layers.cbegin(),
                                          m_required_validation_layers.cend  ());

    // Removes the matching layers.
    for (auto const& layer : supported_layers)
    {
        required_layers.erase(layer.layerName);

        if (required_layers.empty())
            return true;
    }

    // In case some layers could not be found, enumerates them.
    for (auto const& layer : required_layers)
        VulkanDebug::Fatal("Missing validation layer : " + layer + "!");

    return false;
}

RkBool VulkanInstance::CreateInstance() noexcept
{
    // This struct allows us to enable/disable specific validation features.
    VkValidationFeaturesEXT validation_features = {};

    validation_features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validation_features.enabledValidationFeatureCount  = static_cast<RkUint32>(m_enabled_validation_features.size());
    validation_features.pEnabledValidationFeatures     = m_enabled_validation_features.data();
    validation_features.disabledValidationFeatureCount = static_cast<RkUint32>(m_disabled_validation_features.size());
    validation_features.pDisabledValidationFeatures    = m_disabled_validation_features.data();

    // This struct allows us to get debug callbacks during instance creation/destruction.
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {};

    debug_messenger_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_create_info.pNext           = &validation_features;
    debug_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_create_info.pfnUserCallback = VulkanDebug::DebugCallback;

    VkApplicationInfo app_info = {};

    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = RUKEN_PROJECT_NAME;
    app_info.applicationVersion = VK_API_VERSION_1_0;
    app_info.apiVersion         = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instance_create_info = {};

    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    RUKEN_DEBUG
    {
        instance_create_info.pNext = &debug_messenger_create_info;
    }

    instance_create_info.pApplicationInfo        = &app_info;
    instance_create_info.enabledLayerCount       = static_cast<RkUint32>(m_required_validation_layers.size());
    instance_create_info.ppEnabledLayerNames     = m_required_validation_layers.data();
    instance_create_info.enabledExtensionCount   = static_cast<RkUint32>(m_required_extensions.size());
    instance_create_info.ppEnabledExtensionNames = m_required_extensions.data();

    if (VK_ASSERT(vkCreateInstance(&instance_create_info, nullptr, &m_handle)))
        return false;

    VulkanLoader::LoadInstance(m_handle);

    return true;
}

std::vector<RkChar const*> const& VulkanInstance::GetRequiredExtensions() noexcept
{
    return m_required_extensions;
}

std::vector<RkChar const*> const& VulkanInstance::GetRequiredValidationLayers() noexcept
{
    return m_required_validation_layers;
}

std::vector<VkValidationFeatureEnableEXT> const& VulkanInstance::GetEnabledValidationFeatures() noexcept
{
    return m_enabled_validation_features;
}

std::vector<VkValidationFeatureDisableEXT> const& VulkanInstance::GetDisabledValidationFeatures() noexcept
{
    return m_disabled_validation_features;
}

RkBool VulkanInstance::IsValid() const noexcept
{
    return m_handle != nullptr;
}

VkInstance const& VulkanInstance::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion