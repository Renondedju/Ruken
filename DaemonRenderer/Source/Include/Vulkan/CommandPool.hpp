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

BEGIN_DAEMON_NAMESPACE

class Scheduler;

class CommandPool
{
    private:

        struct CommandPoolData
        {
            #pragma region Members

            DAEuint32 primary_index     {0u};
            DAEuint32 secondary_index   {0u};

            std::unique_ptr<VulkanCommandPool> pool;

            std::vector<VulkanCommandBuffer> primary_command_buffers;
            std::vector<VulkanCommandBuffer> second_command_buffers;

            #pragma endregion

            #pragma region Constructors and Destructor

            CommandPoolData() = default;

            CommandPoolData(CommandPoolData const&  in_copy) = delete;
            CommandPoolData(CommandPoolData&&       in_move) = default;

            ~CommandPoolData() = default;

            #pragma endregion

            #pragma region Operators

            CommandPoolData& operator=(CommandPoolData const&   in_copy) = delete;
            CommandPoolData& operator=(CommandPoolData&&        in_move) = delete;

            #pragma endregion
        };

        #pragma region Members

        std::unordered_map<std::thread::id, CommandPoolData> m_command_pools;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit CommandPool(Scheduler& in_scheduler, DAEuint32 in_queue_family_index) noexcept;

        CommandPool(CommandPool const&  in_copy) = delete;
        CommandPool(CommandPool&&       in_move) = default;

        ~CommandPool() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VulkanCommandBuffer* RequestCommandBuffer(VkCommandBufferLevel in_level) noexcept;

        DAEvoid Reset();

        #pragma endregion

        #pragma region Operators

        CommandPool& operator=(CommandPool const&   in_copy) = delete;
        CommandPool& operator=(CommandPool&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE