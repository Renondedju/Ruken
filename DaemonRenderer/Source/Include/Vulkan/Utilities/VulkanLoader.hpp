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

#include "Types/FundamentalTypes.hpp"

#if defined(DAEMON_OS_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <volk/volk.h>

BEGIN_DAEMON_NAMESPACE

class VulkanLoader
{
    public:

        #pragma region Methods

        /**
         * \brief 
         * \note  Call this function before creating the Vulkan instance.
         */
        static DAEvoid Initialize();

        /**
         * \brief Loads global function pointers using application-created 'VkInstance'.
         * \note  Call this function after creating the Vulkan instance.
         */
        static DAEvoid LoadInstance(VkInstance in_instance) noexcept;

        /**
         * \brief Loads global function pointers using application-created 'VkDevice'.
         * \note  Call this function after creating the Vulkan device.
         */
        static DAEvoid LoadDevice(VkDevice in_device) noexcept;

        /**
         * \return The last 'VkInstance' for which global function pointers have been loaded.
         */
        static VkInstance GetLoadedInstance() noexcept;

        /**
         * \return The last 'VkDevice' for which global function pointers have been loaded.
         */
        static VkDevice GetLoadedDevice() noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE