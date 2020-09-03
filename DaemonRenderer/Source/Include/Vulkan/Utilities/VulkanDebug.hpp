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

#include "Vulkan/Utilities/VulkanConfig.hpp"

#include "Debug/Logging/Logger.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class wraps a messenger object which handles passing along debug messages to a provided debug callback.
 *        It also contains helper functions to simplify Vulkan debugging, like checking the result of a vulkan function.
 */
class VulkanDebug
{
    private:

        #pragma region Members

        static Logger*                  m_logger;
        static VkDebugUtilsMessengerEXT m_debug_messenger;

        #pragma endregion

    public:

        #pragma region Methods

        static RkVoid Initialize           (Logger* in_parent_logger) noexcept;
        static RkVoid CreateDebugMessenger ()                         noexcept;
        static RkVoid DestroyDebugMessenger()                         noexcept;

        static RkVoid Debug  (std::string_view in_message) noexcept;
        static RkVoid Info   (std::string_view in_message) noexcept;
        static RkVoid Warning(std::string_view in_message) noexcept;
        static RkVoid Error  (std::string_view in_message) noexcept;
        static RkVoid Fatal  (std::string_view in_message) noexcept;

        /**
         * \return True if the result is an error, else False.
         * \note   Use this methods to check the result of non-critical calls to Vulkan functions.
         */
        static RkBool CheckResult(VkResult in_result, std::string_view in_function_name) noexcept;

        /**
         * \return True if the result is not VK_SUCCESS, else False.
         * \note   Use this method to check the result of critical calls to Vulkan functions.
         */
        static RkBool AssertResult(VkResult in_result, std::string_view in_function_name) noexcept;

        /**
         * \brief Application-defined debug messenger callback function.
         * \param in_message_severity Bitmask specifying which severities of events caused a debug messenger callback.
         * \param in_message_type     Bitmask specifying which types of events caused a debug messenger callback.
         * \param in_callback_data    Structure specifying parameters returned to the callback.
         * \param in_user_data        User data provided when the debug messenger was created.
         */
        static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      in_message_severity,
                                      VkDebugUtilsMessageTypeFlagsEXT             in_message_type,
                                      VkDebugUtilsMessengerCallbackDataEXT const* in_callback_data,
                                      RkVoid*                                    in_user_data);

        /**
         * \param in_type   The type of the object to be named.
         * \param in_handle The object to be named.
         * \param in_name   The name to apply to object.
         */
        static RkVoid SetObjectName(VkObjectType     in_type,
                                     RkUint64        in_handle,
                                     std::string_view in_name) noexcept;

        /**
         * \param in_type   The type of the object to be tagged.
         * \param in_handle The object to be tagged.
         * \param in_name   The numerical identifier of the tag.
         * \param in_size   The number of bytes of data to attach to the object.
         * \param in_tag    The data to be associated with the object.
         */
        static RkVoid SetObjectTag(VkObjectType   in_type,
                                    RkUint64      in_handle,
                                    RkUint64      in_name,
                                    RkSize        in_size,
                                    RkVoid const* in_tag) noexcept;

        static std::string ToString(VkResult                                    in_result)        noexcept;
        static std::string ToString(VkObjectType                                in_object)        noexcept;
        static std::string ToString(VkDebugUtilsMessageTypeFlagsEXT             in_message_type)  noexcept;
        static std::string ToString(VkDebugUtilsMessengerCallbackDataEXT const* in_callback_data) noexcept;

        #pragma endregion
};

/**
 * \return True if the result is an error, else False.
 * \note   Use this methods to check the result of non-critical calls to Vulkan functions.
 */
#define VK_CHECK(result) VulkanDebug::CheckResult(result, #result)

/**
 * \return True if the result is not VK_SUCCESS, else False.
 * \note   Use this method to check the result of critical calls to Vulkan functions.
 */
#define VK_ASSERT(result) VulkanDebug::AssertResult(result, #result)

END_RUKEN_NAMESPACE