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

#include <vector>

#include "Vector/Vector.hpp"

#include "Vulkan/Utilities/VulkanLoader.hpp"

BEGIN_DAEMON_NAMESPACE

class CommandBuffer;
class PhysicalDevice;

class Queue
{
    private:

        #pragma region Members

        PhysicalDevice const* m_physical_device {nullptr};

        VkQueue         m_handle        {nullptr};
        DAEuint32       m_family_index  {UINT_MAX};
        DAEuint32       m_index         {UINT_MAX};
        VkQueueFlags    m_flags         {0u};

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit Queue(PhysicalDevice const*    in_physical_device,
                       VkQueue                  in_handle,
                       DAEuint32                in_family_index,
                       DAEuint32                in_index,
                       VkQueueFlags             in_flags) noexcept;

        Queue(Queue const&  in_copy) = delete;
        Queue(Queue&&       in_move) = default;

        ~Queue() = default;

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

        /**
         * \brief 
         * \param in_present_info 
         */
        [[nodiscard]]
        DAEbool Present(VkPresentInfoKHR const& in_present_info) const noexcept;

        /**
         * \brief
         * \param in_submit_info 
         * \param in_fence       
         */
        [[nodiscard]]
        DAEbool Submit(VkSubmitInfo const& in_submit_info, VkFence in_fence = nullptr) const noexcept;

        [[nodiscard]]
        DAEbool Submit(std::vector<VkSubmitInfo> const& in_submit_infos, VkFence in_fence = nullptr) const noexcept;

        [[nodiscard]]
        DAEbool Submit(CommandBuffer const& in_command_buffer, VkFence in_fence = nullptr) const noexcept;

        /**
         * \brief 
         */
        DAEvoid WaitIdle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkQueue const& GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        DAEuint32 GetFamilyIndex() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        DAEuint32 GetIndex() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkQueueFlags GetFlags() const noexcept;

        [[nodiscard]]
        DAEbool IsPresentationSupported(VkSurfaceKHR const& in_surface) const noexcept;

        #pragma endregion

        #pragma region Operators

        Queue& operator=(Queue const&   in_copy) = delete;
        Queue& operator=(Queue&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE