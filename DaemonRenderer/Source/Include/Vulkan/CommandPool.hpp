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

BEGIN_DAEMON_NAMESPACE

class Device;

class CommandPool
{
    private:

        #pragma region Members

        Device const&   m_device;
        VkCommandPool   m_handle;
        DAEuint32       m_queue_family_index;

        #pragma endregion

        #pragma region Constructor

        explicit CommandPool(Device const&  in_device,
                             DAEuint32      in_queue_family_index) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        CommandPool() = delete;

        CommandPool(CommandPool const&  in_copy) noexcept = delete;
        CommandPool(CommandPool&&       in_move) noexcept;

        ~CommandPool();

        #pragma endregion

        #pragma region Operators

        CommandPool& operator=(CommandPool const&   in_copy) noexcept = delete;
        CommandPool& operator=(CommandPool&&        in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        static DAEbool Create(Device* in_device, VkCommandPoolCreateInfo const& in_create_info, CommandPool** out_command_pool);

        [[nodiscard]] DAEbool AllocateCommandBuffers(VkCommandBufferAllocateInfo& in_allocate_infos, VkCommandBuffer* out_command_buffers) const;

        DAEvoid FreeCommandBuffers(DAEuint32 in_count, VkCommandBuffer* in_command_buffers) const noexcept;

        /**
         * \brief 
         *
         * \param in_trim_flags 
         */
        DAEvoid Trim(VkCommandPoolTrimFlags in_trim_flags = 0u) const noexcept;

        Device const& GetDevice() const noexcept;

        /**
         * \brief
         *
         * \return 
         */
        [[nodiscard]] VkCommandPool const& GetHandle() const noexcept;


        DAEuint32 GetQueueFamilyIndex() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE