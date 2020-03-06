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

#include <Vulkan/vulkan.h>

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

class PhysicalDevice;

class LogicalDevice
{
    private:

        #pragma region Members

        VkDevice    m_handle;
        VkQueue     m_graphics_queue;
        VkQueue     m_present_queue;
        VkQueue     m_compute_queue;
        VkQueue     m_transfer_queue;

        #pragma endregion

        #pragma region Methods

        DAEbool CreateLogicalDevice(PhysicalDevice const* in_physical_device) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        LogicalDevice() = delete;

        explicit LogicalDevice(PhysicalDevice const* in_physical_device);

        LogicalDevice(LogicalDevice const&  in_copy) = delete;
        LogicalDevice(LogicalDevice&&       in_move) = delete;

        ~LogicalDevice() noexcept;

        #pragma endregion

        #pragma region Operators

        LogicalDevice& operator=(LogicalDevice const&   in_copy) = delete;
        LogicalDevice& operator=(LogicalDevice&&        in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        [[nodiscard]] VkDevice GetHandle() const noexcept;

        #pragma endregion 
};

END_DAEMON_NAMESPACE