/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#include <vector>
#include <optional>

#include "Vulkan/Core/VulkanCommandBuffer.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanCommandPool
{
    private:

        #pragma region Members

        VkCommandPool m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit VulkanCommandPool(DAEuint32 in_queue_family, VkCommandPoolCreateFlags in_flags = 0u) noexcept;

        VulkanCommandPool(VulkanCommandPool const&  in_copy) = delete;
        VulkanCommandPool(VulkanCommandPool&&       in_move) noexcept;

        ~VulkanCommandPool() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        std::optional<VulkanCommandBuffer> AllocateCommandBuffer(VkCommandBufferLevel in_level) const noexcept;
        [[nodiscard]]
        std::vector<VulkanCommandBuffer> AllocateCommandBuffers(DAEuint32 in_count, VkCommandBufferLevel in_level) const noexcept;

        DAEvoid Reset(VkCommandPoolResetFlags in_flags = 0u) const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanCommandPool& operator=(VulkanCommandPool const&   in_copy) = delete;
        VulkanCommandPool& operator=(VulkanCommandPool&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE