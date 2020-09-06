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

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a binary 'VkSemaphore' object.
 *        Semaphores are a synchronization primitive that can be used to insert a dependency
 *        between queue operations or between a queue operation and the host.
 *        Binary semaphores have two states - signaled and unsignaled.
 *        A semaphore can be signaled after execution of a queue operation is completed,
 *        and a queue operation can wait for a semaphore to become signaled before it begins execution.
 */
class VulkanSemaphore
{
    protected:

        #pragma region Members

        VkSemaphore m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanSemaphore() noexcept;

        VulkanSemaphore(VulkanSemaphore const& in_copy) = delete;
        VulkanSemaphore(VulkanSemaphore&&      in_move) noexcept;

        virtual ~VulkanSemaphore() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VkSemaphore const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanSemaphore& operator=(VulkanSemaphore const& in_copy) = delete;
        VulkanSemaphore& operator=(VulkanSemaphore&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE