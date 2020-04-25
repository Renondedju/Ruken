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

USING_DAEMON_NAMESPACE

#pragma region Static Members

Logger*                     VulkanDebug::m_logger             {nullptr};
VkDebugUtilsMessengerEXT    VulkanDebug::m_debug_messenger    {nullptr};

#pragma endregion

#pragma region Methods

DAEvoid VulkanDebug::Initialize(Logger& in_parent_logger)
{
    if (m_logger)
        return;

    m_logger = in_parent_logger.AddChild("Vulkan");

    m_logger->SetLevel(ELogLevel::Info);
}

DAEbool VulkanDebug::CheckResult(VkResult const in_result, std::string const& in_function) noexcept
{
    auto const message = in_function + " : " + ToString(in_result);

    if (in_result == VK_SUCCESS)
        m_logger->Debug(message);
    else if (in_result > 0)
        m_logger->Warning(message);
    else
        m_logger->Error(message);

    return in_result >= 0;
}

DAEvoid VulkanDebug::AssertResult(VkResult const in_result, std::string const& in_function) noexcept
{
    auto const message = in_function + " : " + ToString(in_result);

    if (in_result == VK_SUCCESS)
        m_logger->Debug(message);
    else if (in_result > 0)
        m_logger->Warning(message);
    else
    {
        m_logger->Fatal(message);
        
        exit(1);
    }
}

DAEvoid VulkanDebug::CreateDebugMessenger(VkInstance in_instance) noexcept
{
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

    VK_CHECK(vkCreateDebugUtilsMessengerEXT(in_instance, &debug_messenger_info, nullptr, &m_debug_messenger));
}

DAEvoid VulkanDebug::DestroyDebugMessenger(VkInstance in_instance) noexcept
{
    if (m_debug_messenger)
        vkDestroyDebugUtilsMessengerEXT(in_instance, m_debug_messenger, nullptr);
}

VkBool32 VulkanDebug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT const  in_message_severity,
                              VkDebugUtilsMessageTypeFlagsEXT        const  in_message_type,
                              VkDebugUtilsMessengerCallbackDataEXT   const* in_callback_data,
                              DAEvoid*                                      in_user_data)
{
    (DAEvoid)in_user_data;

    std::string message;

    // Checks which types of events caused the debug callback.
    if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
        message += "GENERAL";
    else
    {
        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            message += "VALIDATION";

        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        {
            if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
                message += "|";

            message += "PERFORMANCE";
        }
    }

    DAEchar tmp[2048];

    sprintf_s(tmp, " - Message ID Number %d, Message ID Name %s :\n\n%s", in_callback_data->messageIdNumber, in_callback_data->pMessageIdName, in_callback_data->pMessage);

    message += tmp;

    // Checks which object(s) caused a debug callback.
    if (in_callback_data->objectCount > 0u)
    {
        sprintf_s(tmp, "\n\nObjects - %d :\n", in_callback_data->objectCount);

        message += tmp;

        // The most recent label should be the first one.
        for (DAEuint32 index = 0u; index < in_callback_data->objectCount; ++index)
        {
            sprintf_s(tmp, "\tObject[%d] - %s, Handle = %p, Name = %s\n", index,
                                                                          ToString(in_callback_data->pObjects[index].objectType).c_str(),
                                                                          reinterpret_cast<DAEvoid*>(in_callback_data->pObjects[index].objectHandle),
                                                                          in_callback_data->pObjects[index].pObjectName);

            message += tmp;
        }
    }

    // Checks what are the last command buffer labels encountered before the debug callback.
    if (in_callback_data->cmdBufLabelCount > 0u)
    {
        sprintf_s(tmp, "\n\nCommand Buffer Labels - %d :\n", in_callback_data->cmdBufLabelCount);

        message += tmp;

        // The most recent label should be the first one.
        for (DAEuint32 index = 0u; index < in_callback_data->cmdBufLabelCount; ++index)
        {
            sprintf_s(tmp, "\tLabel[%d] - Name = %s\n", index, in_callback_data->pCmdBufLabels[index].pLabelName);

            message += tmp;
        }
    }

    // Checks what are the last queue labels encountered before the debug callback.
    if (in_callback_data->queueLabelCount > 0u)
    {
        sprintf_s(tmp, "\n\nQueue Labels - %d :\n", in_callback_data->queueLabelCount);

        message += tmp;

        // The most recent label should be the first one.
        for (DAEuint32 index = 0u; index < in_callback_data->queueLabelCount; ++index)
        {
            sprintf_s(tmp, "\tLabel[%d] - Name = %s\n", index, in_callback_data->pQueueLabels[index].pLabelName);

            message += tmp;
        }
    }

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

    return VK_FALSE;
}

DAEvoid VulkanDebug::SetObjectName(VkObjectType const     in_object_type,
                                   DAEuint64    const     in_object_handle,
                                   std::string  const&    in_object_name) noexcept
{
    VkDebugUtilsObjectNameInfoEXT object_info = {};

    object_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    object_info.objectType   = in_object_type;
    object_info.objectHandle = in_object_handle;
    object_info.pObjectName  = in_object_name.c_str();

    vkSetDebugUtilsObjectNameEXT(VulkanLoader::GetLoadedDevice(), &object_info);
}

DAEvoid VulkanDebug::SetObjectTag(VkObjectType const  in_object_type,
                                  DAEuint64    const  in_object_handle,
                                  DAEuint64    const  in_tag_name,
                                  DAEsize      const  in_tag_size,
                                  DAEvoid      const* in_tag) noexcept
{
    VkDebugUtilsObjectTagInfoEXT tag_info = {};

    tag_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
    tag_info.objectType   = in_object_type;
    tag_info.objectHandle = in_object_handle;
    tag_info.tagName      = in_tag_name;
    tag_info.tagSize      = in_tag_size;
    tag_info.pTag         = in_tag;

    vkSetDebugUtilsObjectTagEXT(VulkanLoader::GetLoadedDevice(), &tag_info);
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
        case VK_OBJECT_TYPE_UNKNOWN:                            return "UNKNOWN";
        case VK_OBJECT_TYPE_INSTANCE:                           return "INSTANCE";
        case VK_OBJECT_TYPE_PHYSICAL_DEVICE:                    return "PHYSICAL_DEVICE";
        case VK_OBJECT_TYPE_DEVICE:                             return "DEVICE";
        case VK_OBJECT_TYPE_QUEUE:                              return "QUEUE";
        case VK_OBJECT_TYPE_SEMAPHORE:                          return "SEMAPHORE";
        case VK_OBJECT_TYPE_COMMAND_BUFFER:                     return "COMMAND_BUFFER";
        case VK_OBJECT_TYPE_FENCE:                              return "FENCE";
        case VK_OBJECT_TYPE_DEVICE_MEMORY:                      return "DEVICE_MEMORY";
        case VK_OBJECT_TYPE_BUFFER:                             return "BUFFER";
        case VK_OBJECT_TYPE_IMAGE:                              return "IMAGE";
        case VK_OBJECT_TYPE_EVENT:                              return "EVENT";
        case VK_OBJECT_TYPE_QUERY_POOL:                         return "QUERY_POOL";
        case VK_OBJECT_TYPE_BUFFER_VIEW:                        return "BUFFER_VIEW";
        case VK_OBJECT_TYPE_IMAGE_VIEW:                         return "IMAGE_VIEW";
        case VK_OBJECT_TYPE_SHADER_MODULE:                      return "SHADER_MODULE";
        case VK_OBJECT_TYPE_PIPELINE_CACHE:                     return "PIPELINE_CACHE";
        case VK_OBJECT_TYPE_PIPELINE_LAYOUT:                    return "PIPELINE_LAYOUT";
        case VK_OBJECT_TYPE_RENDER_PASS:                        return "RENDER_PASS";
        case VK_OBJECT_TYPE_PIPELINE:                           return "PIPELINE";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:              return "DESCRIPTOR_SET_LAYOUT";
        case VK_OBJECT_TYPE_SAMPLER:                            return "SAMPLER";
        case VK_OBJECT_TYPE_DESCRIPTOR_POOL:                    return "DESCRIPTOR_POOL";
        case VK_OBJECT_TYPE_DESCRIPTOR_SET:                     return "DESCRIPTOR_SET";
        case VK_OBJECT_TYPE_FRAMEBUFFER:                        return "FRAMEBUFFER";
        case VK_OBJECT_TYPE_COMMAND_POOL:                       return "COMMAND_POOL";
        case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:           return "SAMPLER_YCBCR_CONVERSION";
        case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:         return "DESCRIPTOR_UPDATE_TEMPLATE";
        case VK_OBJECT_TYPE_SURFACE_KHR:                        return "SURFACE_KHR";
        case VK_OBJECT_TYPE_SWAPCHAIN_KHR:                      return "SWAPCHAIN_KHR";
        case VK_OBJECT_TYPE_DISPLAY_KHR:                        return "DISPLAY_KHR";
        case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:                   return "DISPLAY_NODE_KHR";
        case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:          return "DEBUG_REPORT_CALLBACK_EXT";
        case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:          return "DEBUG_UTILS_MESSENGER_EXT";
        case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:         return "ACCELERATION_STRUCTURE_KHR";
        case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:               return "VALIDATION_CACHE_EXT";
        case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL:    return "PERFORMANCE_CONFIGURATION_INTEL";
        case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR:             return "DEFERRED_OPERATION_KHR";
        case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:        return "INDIRECT_COMMANDS_LAYOUT_NV";

        default:
            return "UNKNOWN_OBJECT_TYPE";
    }
}

Logger& VulkanDebug::GetLogger() noexcept
{
    return *m_logger;
}

VkDebugUtilsMessengerEXT const& VulkanDebug::GetDebugMessenger() noexcept
{
    return m_debug_messenger;
}

#pragma endregion