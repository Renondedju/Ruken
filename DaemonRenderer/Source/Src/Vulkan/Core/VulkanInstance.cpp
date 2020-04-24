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

#include "Vulkan/Core/VulkanInstance.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Static Variables

static std::vector<DAEchar const*> RequiredExtensions =
{
    #ifdef DAEMON_OS_WINDOWS

    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,

    #endif

    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

static std::vector<DAEchar const*> RequiredValidationLayers =
{
    #ifdef DAEMON_DEBUG

    "VK_LAYER_KHRONOS_validation"

    #endif
};

static std::vector<VkValidationFeatureEnableEXT> EnabledValidationFeatures =
{
    #ifdef DAEMON_DEBUG

    VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
    VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT

    #endif
};

static std::vector<VkValidationFeatureDisableEXT> DisabledValidationFeatures =
{

};

#pragma endregion

#pragma region Constructor and Destructor

VulkanInstance::VulkanInstance()
{
    CheckInstanceExtensions();
    CheckValidationLayers  ();
    CreateInstance         ();

    VulkanLoader::LoadInstance        (m_handle);
    VulkanDebug ::CreateDebugMessenger(m_handle);
}

VulkanInstance::~VulkanInstance() noexcept
{
    if (!m_handle)
        return;

    VulkanDebug::DestroyDebugMessenger(m_handle);

    vkDestroyInstance(m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

DAEvoid VulkanInstance::CheckInstanceExtensions() noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of global extension properties.
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the global extension properties.
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, supported_extensions.data()));

    std::set<std::string> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
    {
        required_extensions.erase(extension.extensionName);

        if (required_extensions.empty())
            return;
    }

    // In case some extensions could not be found, enumerates them.
    for (auto const& extension : required_extensions)
        VulkanDebug::GetLogger().Fatal("Missing instance extension : " + extension + "!");

    exit(1);
}

DAEvoid VulkanInstance::CheckValidationLayers() noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of layer properties available.
    VK_CHECK(vkEnumerateInstanceLayerProperties(&count, nullptr));

    std::vector<VkLayerProperties> supported_layers(count);

    // Returns the global layer properties
    VK_CHECK(vkEnumerateInstanceLayerProperties(&count, supported_layers.data()));

    std::set<std::string> required_layers(RequiredValidationLayers.cbegin(), RequiredValidationLayers.cend());

    // Removes the matching layers.
    for (auto const& layer : supported_layers)
    {
        required_layers.erase(layer.layerName);

        if (required_layers.empty())
            return;
    }

    // In case some layers could not be found, enumerates them.
    for (auto const& layer : required_layers)
        VulkanDebug::GetLogger().Fatal("Missing validation layer : " + layer + "!");

    exit(1);
}

DAEvoid VulkanInstance::CreateInstance() noexcept
{
    // This struct allows us to enable/disable specific validation features.
    VkValidationFeaturesEXT validation_features = {};

    validation_features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validation_features.enabledValidationFeatureCount  = static_cast<DAEuint32>(EnabledValidationFeatures.size());
    validation_features.pEnabledValidationFeatures     = EnabledValidationFeatures.data();
    validation_features.disabledValidationFeatureCount = static_cast<DAEuint32>(DisabledValidationFeatures.size());
    validation_features.pDisabledValidationFeatures    = DisabledValidationFeatures.data();

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
    app_info.pApplicationName   = DAEMON_PROJECT_NAME;
    app_info.applicationVersion = VK_API_VERSION_1_0;
    app_info.apiVersion         = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instance_create_info = {};

    instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext                   = &debug_messenger_create_info;
    instance_create_info.pApplicationInfo        = &app_info;
    instance_create_info.enabledLayerCount       = static_cast<DAEuint32>(RequiredValidationLayers.size());
    instance_create_info.ppEnabledLayerNames     = RequiredValidationLayers.data();
    instance_create_info.enabledExtensionCount   = static_cast<DAEuint32>(RequiredExtensions.size());
    instance_create_info.ppEnabledExtensionNames = RequiredExtensions.data();

    VK_CHECK(vkCreateInstance(&instance_create_info, nullptr, &m_handle));
}

VkInstance const& VulkanInstance::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion