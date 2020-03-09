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

#include "Rendering/Utilities/Debug.hpp"

#ifdef DAEMON_OS_WINDOWS

#include "Utility/WindowsOS.hpp"

#include <Vulkan/vulkan_win32.h>

#endif

#include "Containers/Set.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Instance::Instance() :
    m_handle                { nullptr },
    m_required_extensions   { VK_KHR_SURFACE_EXTENSION_NAME }
{
    #ifdef DAEMON_OS_WINDOWS

    m_required_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    #endif

    #ifdef DAEMON_DEBUG

    m_required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    m_required_layers.push_back("VK_LAYER_KHRONOS_validation");

    #endif

    if (CheckInstanceExtensions() && CheckValidationLayers() && SetupInstance())
    {
        GRenderer->GetLogger()->Info("Instance created successfully.");

        #ifdef DAEMON_DEBUG

        if (Debug::SetupDebugMessenger(m_handle,
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    |
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                       GRenderer))
        {
            GRenderer->GetLogger()->Info("Debug messenger set up successfully.");
        }

        else
            GRenderer->GetLogger()->Error("Failed to set up debug messenger!");

        #endif
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create instance!");
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

DAEbool Instance::CheckInstanceExtensions() const noexcept
{
    Set<String> required_extensions(m_required_extensions.cbegin(), m_required_extensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : GetSupportedExtensions())
    {
        required_extensions.erase(extension.extensionName);
    }

    // In case some extensions could not be found, enumerates them.
    if (!required_extensions.empty())
    {
        for (auto const& extension : required_extensions)
        {
            GRenderer->GetLogger()->Error("Missing instance extension : " + extension);
        }

        return false;
    }

    return true;
}

DAEbool Instance::CheckValidationLayers() const noexcept
{
    Set<String> required_layers(m_required_layers.cbegin(), m_required_layers.cend());

    // Removes the matching layers.
    for (auto const& layer : GetSupportedLayers())
    {
        required_layers.erase(layer.layerName);
    }

    // In case some layers could not be found, enumerates them.
    if (!required_layers.empty())
    {
        for (auto const& layer : required_layers)
        {
            GRenderer->GetLogger()->Error("Missing validation layer : " + layer);
        }

        return false;
    }

    return true;
}

DAEbool Instance::SetupInstance() noexcept
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

    return vkCreateInstance(&instance_info, nullptr, &m_handle) == VK_SUCCESS;
}

Vector<VkExtensionProperties> Instance::GetSupportedExtensions(DAEchar const* in_layer_name) noexcept
{
    DAEuint32 count;

    vkEnumerateInstanceExtensionProperties(in_layer_name, &count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(count);

    vkEnumerateInstanceExtensionProperties(in_layer_name, &count, supported_extensions.data());

    return supported_extensions;
}

Vector<VkLayerProperties> Instance::GetSupportedLayers() noexcept
{
    DAEuint32 count;

    vkEnumerateInstanceLayerProperties(&count, nullptr);

    std::vector<VkLayerProperties> supported_layers(count);

    vkEnumerateInstanceLayerProperties(&count, supported_layers.data());

    return supported_layers;
}

VkInstance Instance::GetHandle() const noexcept
{
    return m_handle;
}

Vector<DAEchar const*> const& Instance::GetRequiredExtensions() const noexcept
{
    return m_required_extensions;
}

Vector<DAEchar const*> const& Instance::GetRequiredLayers() const noexcept
{
    return m_required_layers;
}

#pragma endregion