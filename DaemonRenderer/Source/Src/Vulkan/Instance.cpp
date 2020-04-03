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

#include <set>

#include "Vulkan/Debug.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/PhysicalDevice.hpp"

#include "Rendering/Renderer.hpp"

#include "Debug/Logging/Logger.hpp"

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

    VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT

    #endif
};

static std::vector<VkValidationFeatureDisableEXT> DisabledValidationFeatures =
{

};

#pragma endregion

#pragma region Constructor and Destructor

Instance::Instance() noexcept:
    m_logger    {GRenderer->GetLogger()->AddChild("Vulkan")},
    m_handle    {nullptr}
{
    m_logger->propagate = true;

    m_logger->SetLevel(ELogLevel::Debug);

    if (volkInitialize() == VK_SUCCESS)
    {
        if (CheckInstanceVersion   () &&
            CheckInstanceExtensions() &&
            CheckValidationLayers  () &&
            SetupInstance          () )
        {
            volkLoadInstanceOnly(m_handle);

            #ifdef DAEMON_DEBUG

            Debug::SetupDebugMessenger(m_handle,
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                       m_logger);

            #endif

            GRenderer->GetLogger()->Info("Instance created successfully.");
        }

        else
            GRenderer->GetLogger()->Fatal("Failed to create instance!");
    }
}

Instance::~Instance() noexcept
{
    if (m_handle)
    {
        #ifdef DAEMON_DEBUG

        Debug::FreeDebugMessenger(m_handle);

        #endif

        vkDestroyInstance(m_handle, nullptr);

        GRenderer->GetLogger()->Info("Instance destroyed.");
    }
}

#pragma endregion

#pragma region Methods

DAEbool Instance::CheckInstanceVersion() noexcept
{
    DAEuint32 api_version = 0u;

    if (vkEnumerateInstanceVersion(&api_version) == VK_SUCCESS && api_version >= VK_API_VERSION_1_2)
        return true;

    GRenderer->GetLogger()->Error("Vulkan 1.2 is not supported!");

    return false;
}

DAEbool Instance::CheckInstanceExtensions() noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of global extension properties.
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    // Returns the global extension properties.
    vkEnumerateInstanceExtensionProperties(nullptr, &count, supported_extensions.data());

    std::set<std::string> required_extensions(RequiredExtensions.cbegin(), RequiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supported_extensions)
        required_extensions.erase(extension.extensionName);

    // In case some extensions could not be found, enumerates them.
    if (!required_extensions.empty())
    {
        for (auto const& extension : required_extensions)
            GRenderer->GetLogger()->Error("Missing instance extension : " + extension + "!");

        return false;
    }

    return true;
}

DAEbool Instance::CheckValidationLayers() noexcept
{
    DAEuint32 count = 0u;

    // Returns the number of layer properties available.
    vkEnumerateInstanceLayerProperties(&count, nullptr);

    std::vector<VkLayerProperties> supported_layers(count);

    // Returns the global layer properties
    vkEnumerateInstanceLayerProperties(&count, supported_layers.data());

    std::set<std::string> required_layers(RequiredValidationLayers.cbegin(), RequiredValidationLayers.cend());

    // Removes the matching layers.
    for (auto const& layer : supported_layers)
        required_layers.erase(layer.layerName);

    // In case some layers could not be found, enumerates them.
    if (!required_layers.empty())
    {
        for (auto const& layer : required_layers)
            GRenderer->GetLogger()->Error("Missing validation layer : " + layer + "!");

        return false;
    }

    return true;
}

DAEbool Instance::SetupInstance() noexcept
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
    debug_messenger_create_info.pfnUserCallback = Debug::DebugCallback;
    debug_messenger_create_info.pUserData       = m_logger;

    VkApplicationInfo app_info = {};

    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = DAEMON_PROJECT_NAME;
    app_info.applicationVersion = VK_API_VERSION_1_0;
    app_info.apiVersion         = VK_API_VERSION_1_1;

    VkInstanceCreateInfo instance_create_info = {};

    instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext                   = &debug_messenger_create_info;
    instance_create_info.pApplicationInfo        = &app_info;
    instance_create_info.enabledLayerCount       = static_cast<DAEuint32>(RequiredValidationLayers.size());
    instance_create_info.ppEnabledLayerNames     = RequiredValidationLayers.data();
    instance_create_info.enabledExtensionCount   = static_cast<DAEuint32>(RequiredExtensions.size());
    instance_create_info.ppEnabledExtensionNames = RequiredExtensions.data();

    return vkCreateInstance(&instance_create_info, nullptr, &m_handle) == VK_SUCCESS;
}

DAEbool Instance::Create(std::unique_ptr<Instance>& out_instance)
{
    out_instance = std::make_unique<Instance>();

    DAEuint32 count = 0u;

    // Returns the number of physical devices accessible to this Vulkan instance.
    vkEnumeratePhysicalDevices(out_instance->GetHandle(), &count, nullptr);

    if (count == 0u)
    {
        GRenderer->GetLogger()->Error("No GPU available!");
        return false;
    }

    std::vector<VkPhysicalDevice> physical_devices(count);

    // Returns the physical devices accessible to this Vulkan instance.
    vkEnumeratePhysicalDevices(out_instance->GetHandle(), &count, physical_devices.data());

    for (auto const& physical_device : physical_devices)
        out_instance->m_physical_devices.emplace_back(PhysicalDevice(*out_instance, physical_device));

    return out_instance->GetHandle() != nullptr;
}

VkInstance const& Instance::GetHandle() const noexcept
{
    return m_handle;
}

std::vector<PhysicalDevice> const& Instance::GetPhysicalDevices() const noexcept
{
    return m_physical_devices;
}

#pragma endregion