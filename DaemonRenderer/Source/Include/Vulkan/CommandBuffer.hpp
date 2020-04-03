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

#include "Vulkan.hpp"

#include "Vector/Vector.hpp"

BEGIN_DAEMON_NAMESPACE

class CommandPool;

class CommandBuffer
{
    private:

        #pragma region Members

        CommandPool*    m_pool;
        VkCommandBuffer m_handle;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        CommandBuffer() = delete;

        explicit CommandBuffer(VkCommandBuffer in_handle) noexcept;

        CommandBuffer(CommandBuffer const&  in_copy) = delete;
        CommandBuffer(CommandBuffer&&       in_move) = delete;

        ~CommandBuffer();

        #pragma endregion

        #pragma region Operators

        CommandBuffer& operator=(CommandBuffer const&   in_copy) = delete;
        CommandBuffer& operator=(CommandBuffer&&        in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Opens a command buffer debug label region.
         *
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        DAEvoid BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept;

        /**
         * \brief Closes a command buffer label region.
         */
        DAEvoid EndLabel() const noexcept;

        /**
         * \brief Inserts a label into a command buffer.
         *
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        DAEvoid InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept;

        DAEvoid Begin(VkCommandBufferUsageFlags in_usage_flags) const noexcept;

        DAEvoid End() const noexcept;

        DAEvoid Reset(VkCommandBufferResetFlags in_reset_flags = 0u) const noexcept;

        [[nodiscard]] VkCommandBuffer const& GetHandle() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE