#include "Build/Info.hpp"
#include "Build/Build.hpp"

#if defined(RUKEN_OS_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR

    #include "Utility/WindowsOS.hpp"
#endif

USING_RUKEN_NAMESPACE
#include "Rendering/RenderContext.hpp"

#include "Debug/Logging/Logger.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

USING_RUKEN_NAMESPACE

#pragma region Static Variables

constexpr std::array g_required_extensions = {
    VK_KHR_DISPLAY_EXTENSION_NAME,
    VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME,
    VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
    VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef RUKEN_OS_WINDOWS
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
};

constexpr std::array g_required_layers = {
    "VK_LAYER_KHRONOS_validation"
};

constexpr std::array g_enabled_validation_features = {
    vk::ValidationFeatureEnableEXT::eBestPractices,
    vk::ValidationFeatureEnableEXT::eDebugPrintf,
    vk::ValidationFeatureEnableEXT::eSynchronizationValidation
};

#pragma endregion

#pragma region Static Methods

static RkUint32 DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT const  in_message_severity,
    VkDebugUtilsMessageTypeFlagsEXT        const  in_message_type,
    VkDebugUtilsMessengerCallbackDataEXT   const* in_callback_data,
    RkVoid*                                       in_user_data)
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

#pragma endregion

#pragma region Constructors

RenderContext::RenderContext(Logger* in_logger) noexcept: m_logger {in_logger}
{
    if (!m_dynamic_loader.success())
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Fatal("Failed to load vulkan-1.dll"))

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_dynamic_loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

    if (!CreateInstance())
        return;

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);

    CreateDebugUtilsMessenger();
}

RenderContext::~RenderContext() noexcept
{
    if (!m_instance)
        return;

    if (m_debug_utils_messenger)
        m_instance.destroy(m_debug_utils_messenger);

    m_instance.destroy();
}

#pragma endregion

#pragma region Methods

RkBool RenderContext::CreateInstance() noexcept
{
    vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info = {
        .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        .messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        .pfnUserCallback = DebugCallback,
        .pUserData       = m_logger
    };

    vk::ValidationFeaturesEXT validation_features = {
        .pNext                         = &messenger_create_info,
        .enabledValidationFeatureCount = static_cast<RkUint32>(g_enabled_validation_features.size()),
        .pEnabledValidationFeatures    = g_enabled_validation_features.data(),
    };

    vk::ApplicationInfo application_info = {
        .pApplicationName   = RUKEN_PROJECT_NAME,
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0),
        .pEngineName        = RUKEN_PROJECT_NAME,
        .engineVersion      = VK_MAKE_API_VERSION(0, 0, 0, 0),
        .apiVersion         = VK_API_VERSION_1_2
    };

    vk::InstanceCreateInfo instance_create_info = {
        .pNext                   = &validation_features,
        .pApplicationInfo        = &application_info,
        .enabledLayerCount       = static_cast<RkUint32>(g_required_layers.size()),
        .ppEnabledLayerNames     = g_required_layers.data(),
        .enabledExtensionCount   = static_cast<RkUint32>(g_required_extensions.size()),
        .ppEnabledExtensionNames = g_required_extensions.data()
    };

    auto [result, value] = createInstance(instance_create_info);

    if (result != vk::Result::eSuccess)
        RUKEN_SAFE_LOGGER_CALL(m_logger, Fatal("Failed to create vulkan instance : " + vk::to_string(result)))
    else
        m_instance = value;

    return result == vk::Result::eSuccess;
}

RkVoid RenderContext::CreateDebugUtilsMessenger() noexcept
{
    vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info = {
        .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        .messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        .pfnUserCallback = DebugCallback,
        .pUserData       = m_logger
    };

    auto [result, value] = m_instance.createDebugUtilsMessengerEXT(debug_utils_messenger_create_info);

    if (result != vk::Result::eSuccess)
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Error("Failed to create vulkan debug utils messenger : " + vk::to_string(result)))

    m_debug_utils_messenger = value;
}

vk::Instance const& RenderContext::GetInstance() const noexcept
{
    return m_instance;
}

#pragma endregion
