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

#pragma once

#include <string>

#include "Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Logger;

class Debug
{
    private:

        #pragma region Members

        static VkDebugUtilsMessengerEXT m_debug_messenger;

        #pragma endregion

    public:

        #pragma region Methods

        /**
         * \param in_instance         The instance the messenger will be used with.
         * \param in_message_severity The bitmask specifying which severity of event(s) will cause this callback to be called.
         * \param in_message_type     The bitmask specifying which type of event(s) will cause this callback to be called.
         * \param in_logger           The logger the callbacks will be sent to.
         * \return True if the debug messenger could be created, else False.
         */
        static DAEbool SetupDebugMessenger(VkInstance                           in_instance,
                                           VkDebugUtilsMessageSeverityFlagsEXT  in_message_severity,
                                           VkDebugUtilsMessageTypeFlagsEXT      in_message_type,
                                           Logger*                              in_logger) noexcept;

        /**
         * \brief Destroys the debug messenger object.
         * \param in_instance The instance where the callback was created.
         */
        static DAEvoid FreeDebugMessenger(VkInstance in_instance) noexcept;

        /**
         * \brief Application-defined debug messenger callback function.
         * \param in_message_severity Bitmask specifying which severities of events caused a debug messenger callback.
         * \param in_message_type     Bitmask specifying which types of events caused a debug messenger callback.
         * \param in_callback_data    Structure specifying parameters returned to the callback.
         * \param in_user_data        User data provided when the debug messenger was created.
         * \return Always 'VK_FALSE' because we don't want the API to abort.
         */
        static DAEuint32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT       in_message_severity,
                                       VkDebugUtilsMessageTypeFlagsEXT              in_message_type,
                                       VkDebugUtilsMessengerCallbackDataEXT const*  in_callback_data,
                                       DAEvoid*                                     in_user_data);
        
        /**
         * \param in_device        The device that created the object.
         * \param in_object_type   The type of the object to be named.
         * \param in_object_handle The object to be named.
         * \param in_object_name   The name to apply to object.
         */
        static DAEvoid SetObjectName(VkDevice       in_device,
                                     VkObjectType   in_object_type,
                                     DAEuint64      in_object_handle,
                                     DAEchar const* in_object_name) noexcept;

        /**
         * \param in_device        The device that created the object.
         * \param in_object_type   The type of the object to be tagged.
         * \param in_object_handle The object to be tagged.
         * \param in_tag_name      The numerical identifier of the tag.
         * \param in_tag_size      The number of bytes of data to attach to the object.
         * \param in_tag           The data to be associated with the object.
         */
        static DAEvoid SetObjectTag(VkDevice        in_device,
                                    VkObjectType    in_object_type,
                                    DAEuint64       in_object_handle,
                                    DAEuint64       in_tag_name,
                                    DAEsize         in_tag_size,
                                    DAEvoid const*  in_tag) noexcept;

        static std::string ToString(VkObjectType in_object) noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE