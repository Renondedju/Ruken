#include <iostream>

#include "Windowing/Window.hpp"
#include "Build/Build.hpp"

#include "Vulkan/Core/VulkanSwapchain.hpp"
#ifdef RUKEN_OS_WINDOWS
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

USING_RUKEN_NAMESPACE
#include "Rendering/RenderContext.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Meta/Safety.hpp"

#include "Debug/Logging/Logger.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

USING_RUKEN_NAMESPACE

constexpr std::array g_required_extensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef RUKEN_OS_WINDOWS
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

constexpr std::array g_required_layers = {
    "VK_LAYER_KHRONOS_validation"
};

static RkUint32 DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      in_message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             in_message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* in_callback_data,
    RkVoid*                                     in_user_data)
{
    Logger const* logger = static_cast<Logger*>(in_user_data);

    if (!logger)
        return VK_FALSE;

    switch (in_message_severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            logger->Debug(in_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            logger->Info(in_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            logger->Warning(in_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            logger->Error(in_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            logger->Debug(in_callback_data->pMessage);
            break;
    }

    return VK_FALSE;
}

RenderContext::RenderContext(Logger* in_logger) noexcept: m_logger {in_logger}
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

    vk::ApplicationInfo app_info = {
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .apiVersion         = VK_API_VERSION_1_1
    };

    vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info = {
        .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        .messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        .pfnUserCallback = DebugCallback,
        .pUserData       = in_logger
    };

    vk::InstanceCreateInfo instance_create_info = {
        .pNext                   = &messenger_create_info,
        .pApplicationInfo        = &app_info,
        .enabledLayerCount       = static_cast<RkUint32>(g_required_layers.size()),
        .ppEnabledLayerNames     = g_required_layers.data(),
        .enabledExtensionCount   = static_cast<RkUint32>(g_required_extensions.size()),
        .ppEnabledExtensionNames = g_required_extensions.data()
    };

    auto [result, instance] = createInstance(instance_create_info);

    if (result == vk::Result::eSuccess)
    {
        m_instance = instance;

        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);

        std::tie(result, m_messenger) = m_instance.createDebugUtilsMessengerEXT(messenger_create_info);
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan instance!"))
}

RenderContext::~RenderContext() noexcept
{
    m_instance.destroyDebugUtilsMessengerEXT(m_messenger);
    m_instance.destroy();
}

vk::Instance const& RenderContext::GetInstance() const noexcept
{
    return m_instance;
}
