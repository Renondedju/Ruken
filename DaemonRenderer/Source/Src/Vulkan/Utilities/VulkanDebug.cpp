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

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_DAEMON_NAMESPACE

#pragma region Static Members

Logger*                  VulkanDebug::m_logger          {nullptr};
VkDebugUtilsMessengerEXT VulkanDebug::m_debug_messenger {nullptr};

#pragma endregion

#pragma region Methods

DAEvoid VulkanDebug::Initialize(Logger* in_parent_logger) noexcept
{
    if (!in_parent_logger)
        return;

    m_logger = in_parent_logger->AddChild("Vulkan");
}

DAEvoid VulkanDebug::Debug(std::string_view const in_message) noexcept
{
    if (m_logger)
        m_logger->Debug(in_message);
}

DAEvoid VulkanDebug::Info(std::string_view const in_message) noexcept
{
    if (m_logger)
        m_logger->Info(in_message);
}

DAEvoid VulkanDebug::Warning(std::string_view const in_message) noexcept
{
    if (m_logger)
        m_logger->Warning(in_message);
}

DAEvoid VulkanDebug::Error(std::string_view const in_message) noexcept
{
    if (m_logger)
        m_logger->Error(in_message);
}

DAEvoid VulkanDebug::Fatal(std::string_view const in_message) noexcept
{
    if (m_logger)
        m_logger->Fatal(in_message);
}

DAEbool VulkanDebug::CheckResult(VkResult const in_result, std::string_view const in_function_name) noexcept
{
    if (in_result == VK_SUCCESS)
        return false;

    #ifdef DAEMON_CONFIG_DEBUG

    auto const message = std::string(in_function_name) + " : " + ToString(in_result);

    if (in_result > VK_SUCCESS)
        m_logger->Warning(message);
    else
        m_logger->Error(message);

    #else

    (DAEvoid)in_function_name;

    #endif

    return in_result < 0;
}

DAEbool VulkanDebug::AssertResult(VkResult const in_result, std::string_view const in_function_name) noexcept
{
    if (in_result == VK_SUCCESS)
        return false;

    #ifdef DAEMON_CONFIG_DEBUG

    auto const message = std::string(in_function_name) + " : " + ToString(in_result);

    m_logger->Fatal(message);

    #else

    (DAEvoid)in_function_name;

    #endif

    return true;
}

DAEvoid VulkanDebug::CreateDebugMessenger() noexcept
{
    #ifdef DAEMON_CONFIG_DEBUG

    if (m_debug_messenger)
        return;

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {};

    debug_messenger_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_info.pfnUserCallback = DebugCallback;

    VK_CHECK(vkCreateDebugUtilsMessengerEXT(VulkanLoader::GetLoadedInstance(), &debug_messenger_info, nullptr, &m_debug_messenger));

    #endif
}

DAEvoid VulkanDebug::DestroyDebugMessenger() noexcept
{
    if (m_debug_messenger)
        vkDestroyDebugUtilsMessengerEXT(VulkanLoader::GetLoadedInstance(), m_debug_messenger, nullptr);
}

VkBool32 VulkanDebug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT const  in_message_severity,
                                    VkDebugUtilsMessageTypeFlagsEXT        const  in_message_type,
                                    VkDebugUtilsMessengerCallbackDataEXT   const* in_callback_data,
                                    DAEvoid*                                      in_user_data)
{
    (DAEvoid)in_user_data;

    #ifdef DAEMON_CONFIG_DEBUG

    if (!m_logger)
        return VK_FALSE;

    auto const message = ToString(in_message_type) + ToString(in_callback_data);

    // Checks which severity of events caused a debug callback.
    switch (in_message_severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            m_logger->Debug(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            m_logger->Info(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            m_logger->Warning(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            m_logger->Error(message);
            break;

        default:
            break;
    }

    #else

    (DAEvoid)in_message_severity;
    (DAEvoid)in_message_type;
    (DAEvoid)in_callback_data;

    #endif

    return VK_FALSE;
}

DAEvoid VulkanDebug::SetObjectName(VkObjectType     const in_type,
                                   DAEuint64        const in_handle,
                                   std::string_view const in_name) noexcept
{
    VkDebugUtilsObjectNameInfoEXT object_info = {};

    object_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    object_info.objectType   = in_type;
    object_info.objectHandle = in_handle;
    object_info.pObjectName  = in_name.data();

    VK_CHECK(vkSetDebugUtilsObjectNameEXT(VulkanLoader::GetLoadedDevice(), &object_info));
}

DAEvoid VulkanDebug::SetObjectTag(VkObjectType const  in_type,
                                  DAEuint64    const  in_handle,
                                  DAEuint64    const  in_name,
                                  DAEsize      const  in_size,
                                  DAEvoid      const* in_tag) noexcept
{
    VkDebugUtilsObjectTagInfoEXT tag_info = {};

    tag_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
    tag_info.objectType   = in_type;
    tag_info.objectHandle = in_handle;
    tag_info.tagName      = in_name;
    tag_info.tagSize      = in_size;
    tag_info.pTag         = in_tag;

    VK_CHECK(vkSetDebugUtilsObjectTagEXT(VulkanLoader::GetLoadedDevice(), &tag_info));
}

std::string VulkanDebug::ToString(VkResult const in_result) noexcept
{
    switch (in_result)
    {
        case VK_SUCCESS:                                            return "SUCCESS";
        case VK_NOT_READY:                                          return "NOT_READY";
        case VK_TIMEOUT:                                            return "TIMEOUT";
        case VK_EVENT_SET:                                          return "EVENT_SET";
        case VK_EVENT_RESET:                                        return "EVENT_RESET";
        case VK_INCOMPLETE:                                         return "INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:                           return "ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:                         return "ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:                        return "ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:                                  return "ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:                            return "ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:                            return "ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:                        return "ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:                          return "ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:                          return "ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:                             return "ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:                         return "ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:                              return "ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:                                      return "ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:                           return "ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:                      return "ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:                                return "ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:               return "ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_ERROR_SURFACE_LOST_KHR:                             return "ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:                     return "ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:                                     return "SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:                              return "ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:                     return "ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:                        return "ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:                            return "ERROR_INVALID_SHADER_NV";
        case VK_ERROR_INCOMPATIBLE_VERSION_KHR:                     return "ERROR_INCOMPATIBLE_VERSION_KHR";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_EXT:                            return "ERROR_NOT_PERMITTED_EXT";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:          return "ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR:                                    return "THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:                                    return "THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:                             return "OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:                         return "OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:                return "ERROR_PIPELINE_COMPILE_REQUIRED_EXT";

        default:
            return "UNKNOWN_RESULT";
    }
}

std::string VulkanDebug::ToString(VkObjectType const in_object) noexcept
{
    switch (in_object)
    {
        case VK_OBJECT_TYPE_UNKNOWN:                         return "UNKNOWN";
        case VK_OBJECT_TYPE_INSTANCE:                        return "INSTANCE";
        case VK_OBJECT_TYPE_PHYSICAL_DEVICE:                 return "PHYSICAL_DEVICE";
        case VK_OBJECT_TYPE_DEVICE:                          return "DEVICE";
        case VK_OBJECT_TYPE_QUEUE:                           return "QUEUE";
        case VK_OBJECT_TYPE_SEMAPHORE:                       return "SEMAPHORE";
        case VK_OBJECT_TYPE_COMMAND_BUFFER:                  return "COMMAND_BUFFER";
        case VK_OBJECT_TYPE_FENCE:                           return "FENCE";
        case VK_OBJECT_TYPE_DEVICE_MEMORY:                   return "DEVICE_MEMORY";
        case VK_OBJECT_TYPE_BUFFER:                          return "BUFFER";
        case VK_OBJECT_TYPE_IMAGE:                           return "IMAGE";
        case VK_OBJECT_TYPE_EVENT:                           return "EVENT";
        case VK_OBJECT_TYPE_QUERY_POOL:                      return "QUERY_POOL";
        case VK_OBJECT_TYPE_BUFFER_VIEW:                     return "BUFFER_VIEW";
        case VK_OBJECT_TYPE_IMAGE_VIEW:                      return "IMAGE_VIEW";
        case VK_OBJECT_TYPE_SHADER_MODULE:                   return "SHADER_MODULE";
        case VK_OBJECT_TYPE_PIPELINE_CACHE:                  return "PIPELINE_CACHE";
        case VK_OBJECT_TYPE_PIPELINE_LAYOUT:                 return "PIPELINE_LAYOUT";
        case VK_OBJECT_TYPE_RENDER_PASS:                     return "RENDER_PASS";
        case VK_OBJECT_TYPE_PIPELINE:                        return "PIPELINE";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:           return "DESCRIPTOR_SET_LAYOUT";
        case VK_OBJECT_TYPE_SAMPLER:                         return "SAMPLER";
        case VK_OBJECT_TYPE_DESCRIPTOR_POOL:                 return "DESCRIPTOR_POOL";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET:                  return "DESCRIPTOR_SET";
        case VK_OBJECT_TYPE_FRAMEBUFFER:                     return "FRAMEBUFFER";
        case VK_OBJECT_TYPE_COMMAND_POOL:                    return "COMMAND_POOL";
        case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:        return "SAMPLER_YCBCR_CONVERSION";
        case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:      return "DESCRIPTOR_UPDATE_TEMPLATE";
        case VK_OBJECT_TYPE_SURFACE_KHR:                     return "SURFACE_KHR";
        case VK_OBJECT_TYPE_SWAPCHAIN_KHR:                   return "SWAPCHAIN_KHR";
        case VK_OBJECT_TYPE_DISPLAY_KHR:                     return "DISPLAY_KHR";
        case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:                return "DISPLAY_NODE_KHR";
        case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:       return "DEBUG_REPORT_CALLBACK_EXT";
        case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:       return "DEBUG_UTILS_MESSENGER_EXT";
        case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:      return "ACCELERATION_STRUCTURE_KHR";
        case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:            return "VALIDATION_CACHE_EXT";
        case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL: return "PERFORMANCE_CONFIGURATION_INTEL";
        case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR:          return "DEFERRED_OPERATION_KHR";
        case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:     return "INDIRECT_COMMANDS_LAYOUT_NV";

        default:
            return "UNKNOWN_OBJECT_TYPE";
    }
}

std::string VulkanDebug::ToString(VkDebugUtilsMessageTypeFlagsEXT const in_message_type) noexcept
{
    std::string output;

    // Checks which types of events caused the debug callback.
    if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
        output += "GENERAL";
    else
    {
        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            output += "VALIDATION";

        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        {
            if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
                output += "|";

            output += "PERFORMANCE";
        }
    }

    return output;
}

std::string VulkanDebug::ToString(VkDebugUtilsMessengerCallbackDataEXT const* in_callback_data) noexcept
{
    // TODO : needs to look for a better way to format the message.
    std::string output;

    output += " - Message ID Number : ";
    output += std::to_string(in_callback_data->messageIdNumber);
    output += " | Message ID Name : ";
    output += in_callback_data->pMessageIdName;
    output += "\n\t";
    output += in_callback_data->pMessage;

    // Checks which object(s) caused a debug callback.
    if (in_callback_data->objectCount > 0u)
    {
        output += "\n\n\tObjects - ";
        output += std::to_string(in_callback_data->objectCount);

        // The most recent label should be the first one.
        for (auto index = 0u; index < in_callback_data->objectCount; ++index)
        {
            output += "\n\t\tObject[";
            output += std::to_string(index);
            output += "] - ";
            output += ToString(in_callback_data->pObjects[index].objectType);
            output += " | Handle ";
            output += std::to_string(in_callback_data->pObjects[index].objectHandle);
            output += " | Name ";

            if (in_callback_data->pObjects[index].pObjectName)
                output += in_callback_data->pObjects[index].pObjectName;
        }
    }

    // Checks what are the last command buffer labels encountered before the debug callback.
    if (in_callback_data->cmdBufLabelCount > 0u)
    {
        output += "\n\n\tCommand Buffer Labels - ";
        output += std::to_string(in_callback_data->cmdBufLabelCount);

        // The most recent label should be the first one.
        for (auto index = 0u; index < in_callback_data->cmdBufLabelCount; ++index)
        {
            output += "\n\t\tLabel[";
            output += std::to_string(index);
            output += "] - ";
            output += in_callback_data->pCmdBufLabels[index].pLabelName;
        }
    }

    // Checks what are the last queue labels encountered before the debug callback.
    if (in_callback_data->queueLabelCount > 0u)
    {
        output += "\n\n\tQueue Labels - ";
        output += std::to_string(in_callback_data->queueLabelCount);

        // The most recent label should be the first one.
        for (auto index = 0u; index < in_callback_data->queueLabelCount; ++index)
        {
            output += "\n\t\tLabel[";
            output += std::to_string(index);
            output += "] - ";
            output += in_callback_data->pCmdBufLabels[index].pLabelName;
        }
    }

    return output;
}

#pragma endregion