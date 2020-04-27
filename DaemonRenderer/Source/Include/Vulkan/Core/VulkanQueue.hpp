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

#include "Vector/Vector.hpp"

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanCommandBuffer;

class VulkanQueue
{
    private:

        #pragma region Members

        VkPhysicalDevice    m_physical_device   {nullptr};
        VkQueue             m_handle            {nullptr};
        DAEuint32           m_queue_family      {UINT_MAX};

        mutable std::mutex m_mutex;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit VulkanQueue(VkPhysicalDevice   in_physical_device,
                             VkQueue            in_handle,
                             DAEuint32          in_queue_family) noexcept;

        VulkanQueue(VulkanQueue const&  in_copy) = delete;
        VulkanQueue(VulkanQueue&&       in_move) noexcept;

        ~VulkanQueue() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Opens a queue debug label region.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        DAEvoid BeginLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept;

        /**
         * \brief Closes a queue debug label region.
         */
        DAEvoid EndLabel() const noexcept;

        /**
         * \brief Inserts a label into a queue.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        DAEvoid InsertLabel(DAEchar const* in_label_name, Vector4f const& in_color) const noexcept; 

        DAEvoid Submit(VkSubmitInfo const& in_submit_info, VkFence in_fence = nullptr) const noexcept;

        DAEvoid Submit(std::vector<VkSubmitInfo> const& in_submit_infos, VkFence in_fence = nullptr) const noexcept;

        DAEvoid Submit(VulkanCommandBuffer const& in_command_buffer, VkFence in_fence = nullptr) const noexcept;

        DAEvoid Present(VkPresentInfoKHR const& in_present_info) const noexcept;

        DAEvoid WaitIdle() const noexcept;

        [[nodiscard]]
        VkQueue const& GetHandle() const noexcept;
        [[nodiscard]]
        DAEuint32 GetQueueFamily() const noexcept;
        [[nodiscard]]
        DAEbool IsPresentationSupported(VkSurfaceKHR in_surface) const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanQueue& operator=(VulkanQueue const&   in_copy) = delete;
        VulkanQueue& operator=(VulkanQueue&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE