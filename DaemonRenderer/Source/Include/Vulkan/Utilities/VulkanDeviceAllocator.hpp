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

#include "Vulkan/Core/VulkanImage.hpp"
#include "Vulkan/Core/VulkanBuffer.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class wraps memory allocation and resource (buffer and image) creation in Vulkan.
 */
class VulkanDeviceAllocator
{
    private:

        #pragma region Members

        VmaAllocator m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanDeviceAllocator(class VulkanPhysicalDevice const& in_physical_device) noexcept;

        VulkanDeviceAllocator(VulkanDeviceAllocator const& in_copy) = delete;
        VulkanDeviceAllocator(VulkanDeviceAllocator&&      in_move) = delete;

        ~VulkanDeviceAllocator() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        std::optional<VulkanImage> CreateImage(VkImageCreateInfo       const& in_image_create_info,
                                               VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        [[nodiscard]]
        std::optional<VulkanBuffer> CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                                                 VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        [[nodiscard]]
        DAEbool IsValid() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanDeviceAllocator& operator=(VulkanDeviceAllocator const& in_copy) = delete;
        VulkanDeviceAllocator& operator=(VulkanDeviceAllocator&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE