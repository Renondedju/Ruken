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

#include <GLFW/glfw3.h>

#include "Rendering/Renderer.hpp"

#include "Containers/Set.hpp"

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::Renderer* DAEMON_NAMESPACE::GRenderer = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

USING_DAEMON_NAMESPACE

#pragma region Constructor

Renderer::Renderer() noexcept :
    m_logger    { GRootLogger->AddChild("Rendering") },
    m_instance  { nullptr },
    m_messenger { nullptr }
{
    DAEuint32 count;

    auto const extensions = glfwGetRequiredInstanceExtensions(&count);

    for (DAEuint32 i = 0u; i < count; ++i)
    {
        m_required_extensions.emplace_back(extensions[i]);
    }

    #ifdef DAEMON_DEBUG

    m_required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    #endif

    CheckInstanceExtensions();

    #ifdef DAEMON_DEBUG

    m_required_layers.push_back("VK_LAYER_KHRONOS_validation");

    CheckValidationLayers();

    #endif

    CreateInstance();
}

Renderer::~Renderer() noexcept
{
    vkDestroyInstance(m_instance, nullptr);
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
    instance_info.enabledLayerCount       = static_cast<DAEuint32>(m_required_layers.size());
    instance_info.ppEnabledLayerNames     = m_required_layers.data();
    instance_info.enabledExtensionCount   = static_cast<DAEuint32>(m_required_extensions.size());
    instance_info.ppEnabledExtensionNames = m_required_extensions.data();

    if (vkCreateInstance(&instance_info, nullptr, &m_instance) == VK_SUCCESS)
    {
        m_logger->Info("Instance created successfully.");
    }

    else
        m_logger->Fatal("Failed to create instance!");
}

DAEvoid Renderer::SetupDebugMessenger() noexcept
{
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info;

    debug_messenger_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_info.pNext           = nullptr;
    debug_messenger_info.flags           = 0u;
    debug_messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    |
                                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_info.pfnUserCallback = DebugCallback;
    debug_messenger_info.pUserData       = this;

    vkCreateDebugUtilsMessengerEXT(m_instance, &debug_messenger_info, nullptr, &m_messenger);
}

DAEvoid Renderer::CheckInstanceExtensions() const noexcept
{
    // Returns up to requested number of global extension properties.
    DAEuint32 extension_count = 0u;

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(extension_count);

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, supported_extensions.data());

    // Checks if all the required extensions are supported.
    std::set<String> required_extensions(m_required_extensions.cbegin(), m_required_extensions.cend());

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
    std::set<String> required_layers(m_required_layers.cbegin(), m_required_layers.cend());

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

Logger const* Renderer::GetLogger() const noexcept
{
    return m_logger;
}

#pragma endregion

VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT       in_message_severity,
                       VkDebugUtilsMessageTypeFlagsEXT              in_message_type,
                       VkDebugUtilsMessengerCallbackDataEXT const*  in_callback_data,
                       DAEvoid*                                     in_user_data)
{
    Renderer* renderer = static_cast<Renderer*>(in_user_data);

    renderer->GetLogger()->Info(in_callback_data->pMessage);

    return VK_FALSE;
}