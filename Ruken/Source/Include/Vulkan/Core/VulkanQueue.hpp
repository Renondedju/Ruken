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

#include <mutex>
#include <vector>

#include "Maths/Vector/Vector.hpp"

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

class VulkanCommandBuffer;
class VulkanSemaphore;
class VulkanSwapchain;

/**
 * \brief RAII-class wrapping a 'VkQueue' object.
 *        Creating a logical device also creates the queues associated with that device.
 *        Command buffers must be submitted to a queue for execution.
 * \note  A queue does not always support graphics, compute and transfer operations.
 */
class VulkanQueue
{
    private:

        #pragma region Members

        VkPhysicalDevice m_physical_device {nullptr};
        VkQueue          m_handle          {nullptr};
        RkUint32        m_queue_family    {UINT_MAX};

        mutable std::mutex m_mutex {};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanQueue(VkPhysicalDevice in_physical_device,
                             VkQueue          in_handle,
                             RkUint32        in_queue_family) noexcept;

        VulkanQueue(VulkanQueue const& in_copy) = delete;
        VulkanQueue(VulkanQueue&&      in_move) noexcept;

        ~VulkanQueue() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Waits on the host for the completion of outstanding queue operations on this queue.
         */
        RkVoid WaitIdle() const noexcept;

        /**
         * \brief Opens a queue debug label region.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        RkVoid BeginLabel(RkChar const* in_label_name, Vector4f const& in_color) const noexcept;

        /**
         * \brief Inserts a label into a queue.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        RkVoid InsertLabel(RkChar const* in_label_name, Vector4f const& in_color) const noexcept; 

        /**
         * \brief Closes a queue debug label region.
         */
        RkVoid EndLabel() const noexcept;

        /**
         * \brief Submits a single command buffer and an optional fence for execution.
         * \note  Use this function to quickly submit a transfer operation.
         */
        RkBool Submit(VulkanCommandBuffer const& in_command_buffer, VkFence in_fence = nullptr) const noexcept;

        /**
         * \brief Submits a sequence of semaphores or command buffers and an optional fence for execution.
         */
        RkBool Submit(VkSubmitInfo const& in_submit_info, VkFence in_fence = nullptr) const noexcept;

        /**
         * \brief Submits multiple sequences of semaphores or command buffers and an optional fence for execution.
         */
        RkBool Submit(std::vector<VkSubmitInfo> const& in_submit_infos, VkFence in_fence = nullptr) const noexcept;

        /**
         * \return True if an image could be queued for presentation, else False.
         */
        RkBool Present(VulkanSwapchain const& in_swapchain, VulkanSemaphore const& in_semaphore) const noexcept;

        /**
         * \return True if an image could be queued for presentation, else False.
         */
        RkBool Present(VkPresentInfoKHR const& in_present_info) const noexcept;

        /**
         * \return True if presentation is supported to the given surface, else False.
         */
        [[nodiscard]]
        RkBool IsPresentationSupported(VkSurfaceKHR in_surface) const noexcept;

        [[nodiscard]] VkQueue const& GetHandle     () const noexcept;
        [[nodiscard]] RkUint32      GetQueueFamily() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanQueue& operator=(VulkanQueue const& in_copy) = delete;
        VulkanQueue& operator=(VulkanQueue&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE