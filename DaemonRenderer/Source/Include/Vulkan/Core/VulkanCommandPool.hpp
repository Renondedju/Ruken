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

#include <optional>

#include "Vulkan/Core/VulkanCommandBuffer.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkCommandPool' object.
 *        Command pools are opaque objects that command buffer memory is allocated from,
 *        and which allow the implementation to amortize the cost of resource creation across multiple command buffers.
 *        Command pools are externally synchronized, meaning that a command pool must not be used concurrently in multiple threads.
 *        That includes use via recording commands on any command buffers allocated from the pool,
 *        as well as operations that allocate, free, and reset command buffers or the pool itself.
 */
class VulkanCommandPool
{
    private:

        #pragma region Members

        VkCommandPool m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanCommandPool(DAEuint32 in_queue_family, VkCommandPoolCreateFlags in_flags = 0u) noexcept;

        VulkanCommandPool(VulkanCommandPool const& in_copy) = delete;
        VulkanCommandPool(VulkanCommandPool&&      in_move) noexcept;

        ~VulkanCommandPool() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return A command buffer if one could be allocated from this pool.
         * \note   The command buffers allocated using this function are single use,
         *         meaning it can submitted multiple times but only recorded once.
         */
        [[nodiscard]]
        std::optional<VulkanCommandBuffer> AllocateCommandBuffer(VkCommandBufferLevel in_level) const noexcept;

        [[nodiscard]]
        VkCommandPool const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanCommandPool& operator=(VulkanCommandPool const& in_copy) = delete;
        VulkanCommandPool& operator=(VulkanCommandPool&&      in_move) noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE