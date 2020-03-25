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

#include <memory>

#include "Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Buffer;
class Device;

class BufferView
{
    private:

        #pragma region Members

        Device*                 m_device;
        Buffer*                 m_buffer;
        VkBufferView            m_handle;
        VkBufferViewCreateInfo  m_info;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        BufferView() = delete;

        BufferView(Device*                          in_device,
                   Buffer*                          in_buffer,
                   VkBufferView                     in_handle,
                   VkBufferViewCreateInfo const&    in_info) noexcept;

        BufferView(BufferView const&    in_copy) noexcept = delete;
        BufferView(BufferView&&         in_move) noexcept = default;

        ~BufferView();

        #pragma endregion

        #pragma region Operators

        BufferView& operator=(BufferView const& in_copy) noexcept = delete;
        BufferView& operator=(BufferView&&      in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        static DAEbool Create(Device*                       in_device,
                              Buffer*                       in_buffer,
                              VkBufferViewCreateInfo const& in_create_info,
                              std::unique_ptr<BufferView>&  out_buffer_view) noexcept;

        [[nodiscard]] VkBufferView GetHandle() const noexcept;

        [[nodiscard]] VkFormat GetFormat() const noexcept;

        [[nodiscard]] VkDeviceSize GetOffset() const noexcept;

        [[nodiscard]] VkDeviceSize GetRange() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE