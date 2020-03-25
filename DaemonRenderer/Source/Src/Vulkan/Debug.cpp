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

#include "Vulkan/Debug.hpp"

#include "Containers/String.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

VkDebugUtilsMessengerEXT Debug::m_debug_messenger = nullptr;

#pragma region Methods

DAEbool Debug::SetupDebugMessenger(VkInstance                                   in_instance,
                                   VkDebugUtilsMessageSeverityFlagsEXT const    in_message_severity,
                                   VkDebugUtilsMessageTypeFlagsEXT     const    in_message_type,
                                   Logger*                                      in_logger) noexcept
{
    if (m_debug_messenger)
    {
        FreeDebugMessenger(in_instance);
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {};

    debug_messenger_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_info.messageSeverity = in_message_severity;
    debug_messenger_info.messageType     = in_message_type;
    debug_messenger_info.pfnUserCallback = DebugCallback;
    debug_messenger_info.pUserData       = in_logger;

    return vkCreateDebugUtilsMessengerEXT(in_instance, &debug_messenger_info, nullptr, &m_debug_messenger) == VK_SUCCESS;
}

DAEvoid Debug::FreeDebugMessenger(VkInstance in_instance) noexcept
{
    vkDestroyDebugUtilsMessengerEXT(in_instance, m_debug_messenger, nullptr);
}

DAEuint32 Debug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT const     in_message_severity,
                              VkDebugUtilsMessageTypeFlagsEXT         const     in_message_type,
                              VkDebugUtilsMessengerCallbackDataEXT    const*    in_callback_data,
                              DAEvoid*                                          in_user_data)
{
    String message;

    // Checks which types of events caused the debug callback.
    if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
    {
        message += "GENERAL";
    }

    else
    {
        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        {
            message += "VALIDATION";
        }

        if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        {
            if (in_message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            {
                message += "|";
            }

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
                                                                          ToString(in_callback_data->pObjects[index].objectType),
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

    Logger const* logger = reinterpret_cast<Logger*>(in_user_data);

    // Checks which severity of events caused a debug callback.
    switch (in_message_severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            logger->Debug(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            logger->Info(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            logger->Warning(message);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            logger->Error(message);
            break;

        default:
            break;
    }

    return VK_FALSE;
}

DAEvoid Debug::SetObjectName(VkDevice               in_device,
                             VkObjectType const     in_object_type,
                             DAEuint64    const     in_object_handle,
                             DAEchar      const*    in_object_name) noexcept
{
    VkDebugUtilsObjectNameInfoEXT object_info = {};

    object_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    object_info.objectType   = in_object_type;
    object_info.objectHandle = in_object_handle;
    object_info.pObjectName  = in_object_name;

    vkSetDebugUtilsObjectNameEXT(in_device, &object_info);
}

DAEvoid Debug::SetObjectTag(VkDevice            in_device,
                            VkObjectType const  in_object_type,
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

    vkSetDebugUtilsObjectTagEXT(in_device, &tag_info);
}

DAEchar const* Debug::ToString(VkObjectType const in_object) noexcept
{
    switch (in_object)
    {
    #define OBJECT(object) case VK_OBJECT_TYPE_##object: return #object;
            OBJECT(UNKNOWN);
            OBJECT(INSTANCE);
            OBJECT(PHYSICAL_DEVICE);
            OBJECT(DEVICE);
            OBJECT(QUEUE);
            OBJECT(SEMAPHORE);
            OBJECT(COMMAND_BUFFER);
            OBJECT(FENCE);
            OBJECT(DEVICE_MEMORY);
            OBJECT(BUFFER);
            OBJECT(IMAGE);
            OBJECT(EVENT);
            OBJECT(QUERY_POOL);
            OBJECT(BUFFER_VIEW);
            OBJECT(IMAGE_VIEW);
            OBJECT(SHADER_MODULE);
            OBJECT(PIPELINE_CACHE);
            OBJECT(PIPELINE_LAYOUT);
            OBJECT(RENDER_PASS);
            OBJECT(PIPELINE);
            OBJECT(DESCRIPTOR_SET_LAYOUT);
            OBJECT(SAMPLER);
            OBJECT(DESCRIPTOR_POOL);
            OBJECT(DESCRIPTOR_SET);
            OBJECT(FRAMEBUFFER);
            OBJECT(COMMAND_POOL);
            OBJECT(SAMPLER_YCBCR_CONVERSION);
            OBJECT(DESCRIPTOR_UPDATE_TEMPLATE);
            OBJECT(SURFACE_KHR);
            OBJECT(SWAPCHAIN_KHR);
            OBJECT(DISPLAY_KHR);
            OBJECT(DISPLAY_MODE_KHR);
            OBJECT(DEBUG_REPORT_CALLBACK_EXT);
            OBJECT(OBJECT_TABLE_NVX);
            OBJECT(INDIRECT_COMMANDS_LAYOUT_NVX);
            OBJECT(DEBUG_UTILS_MESSENGER_EXT);
            OBJECT(VALIDATION_CACHE_EXT);
            OBJECT(ACCELERATION_STRUCTURE_NV);
            OBJECT(PERFORMANCE_CONFIGURATION_INTEL);
    #undef  OBJECT
    }

    return "UNKNOWN_OBJECT_TYPE";
}

#pragma endregion