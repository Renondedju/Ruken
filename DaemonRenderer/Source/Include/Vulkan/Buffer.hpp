﻿/*
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

#include "Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Device;

class Buffer
{
    private:

        #pragma region Members

        Device*         m_device;
        VkBuffer        m_handle;
        VmaAllocation   m_allocation;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Buffer() = delete;

        explicit Buffer(Device*         in_device,
                        VkBuffer        in_handle,
                        VmaAllocation   in_allocation) noexcept;

        Buffer(Buffer const&    in_copy) noexcept = delete;
        Buffer(Buffer&&         in_move) noexcept = default;

        ~Buffer() = default;

        #pragma endregion

        #pragma region Operators

        Buffer& operator=(Buffer const& in_copy) noexcept = delete;
        Buffer& operator=(Buffer&&      in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] Device* GetDevice() const noexcept;

        [[nodiscard]] VkBuffer GetHandle() const noexcept;

        [[nodiscard]] VmaAllocation GetAllocation() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE