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

#include <thread>
#include <unordered_map>

#include "Vulkan/Core/VulkanCommandPool.hpp"

BEGIN_RUKEN_NAMESPACE

class Scheduler;

/**
 * \brief High level interface managing command buffer allocation.
 *        Command buffers are cached to improve performance, but they are single use.
 *        The "Reset" function must be called on this pool in order to reuse its command buffers.
 * \note  A VulkanCommandPool is created for each thread managed by the Scheduler and the main thread.
 */
class CommandPool
{
    private:

        struct CommandPoolData
        {
            DAEuint32 primary_index   {0u};
            DAEuint32 secondary_index {0u};

            std::unique_ptr<VulkanCommandPool> pool {};

            std::vector<std::unique_ptr<VulkanCommandBuffer>> primary_command_buffers {};
            std::vector<std::unique_ptr<VulkanCommandBuffer>> second_command_buffers  {};
        };

        #pragma region Members

        std::unordered_map<std::thread::id, CommandPoolData> m_command_pools;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit CommandPool(Scheduler& in_scheduler, DAEuint32 in_queue_family_index) noexcept;

        CommandPool(CommandPool const& in_copy) = delete;
        CommandPool(CommandPool&&      in_move) = delete;

        ~CommandPool() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief  Requests a single use command buffer for the current thread.
         * \return A pointer to a command buffer if one is available or could be allocated.
         * \note   This function is safe to call from any thread managed by the Scheduler.
         */
        [[nodiscard]]
        VulkanCommandBuffer* RequestCommandBuffer(VkCommandBufferLevel in_level) noexcept;

        /**
         * \brief Resets all managed command buffers, meaning previously requested command buffers can be reused.
         * \note  This function must be called once at the beginning of a frame.
         */
        [[nodiscard]]
        DAEbool Reset() noexcept;

        #pragma endregion

        #pragma region Operators

        CommandPool& operator=(CommandPool const& in_copy) = delete;
        CommandPool& operator=(CommandPool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE