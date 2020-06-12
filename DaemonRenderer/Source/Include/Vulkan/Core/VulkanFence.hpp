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

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkFence' object.
 *        Fences are a synchronization primitive that can be used to insert a dependency from a queue to the host.
 *        Fences have two states - signaled and unsignaled.
 *        A fence can be signaled as part of the execution of a queue submission command.
 *        Fences can be unsignaled on the host with "Reset".
 *        Fences can be waited on by the host with the "Wait" command, and the current state can be queried with "IsSignaled".
 */
class VulkanFence
{
    private:

        #pragma region Members

        VkFence m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanFence() noexcept;

        VulkanFence(VulkanFence const& in_copy) = delete;
        VulkanFence(VulkanFence&&      in_move) noexcept;

        ~VulkanFence() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if this fence could be reset to the unsignaled state, else False.
         */
        [[nodiscard]]
        DAEbool Reset() const noexcept;

        /**
         * \brief If the condition is satisfied when this function is called, then it returns immediately.
         *        If the condition is not satisfied at the time this function is called,
         *        then this function will block and wait for the condition to become satisfied.
         */
        DAEvoid Wait() const noexcept;

        [[nodiscard]] DAEbool        IsSignaled() const noexcept;
        [[nodiscard]] VkFence const& GetHandle () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanFence& operator=(VulkanFence const& in_copy) = delete;
        VulkanFence& operator=(VulkanFence&&      in_move) noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE