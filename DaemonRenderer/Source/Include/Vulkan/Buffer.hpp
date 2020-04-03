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

#include "Vulkan/Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Device;

class Buffer
{
    private:

        #pragma region Members

        Device const&       m_device;
        VkBuffer            m_handle;
        VmaAllocation       m_allocation;
        VmaAllocationInfo   m_allocation_info;
        DAEbool             m_is_mapped;
        DAEbool             m_is_persistent;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit Buffer(Device                  const& in_device,
                        VkBufferCreateInfo      const& in_buffer_create_info,
                        VmaAllocationCreateInfo const& in_allocation_create_info) noexcept;

        explicit Buffer(Device const&               in_device,
                        VkDeviceSize                in_size,
                        VkBufferUsageFlags          in_buffer_usage,
                        VmaAllocationCreateFlags    in_flags,
                        VmaMemoryUsage              in_memory_usage) noexcept;

        explicit Buffer(Device const&                   in_device,
                        VkDeviceSize                    in_size,
                        VkBufferUsageFlags              in_buffer_usage,
                        VmaAllocationCreateFlags        in_flags,
                        VmaMemoryUsage                  in_memory_usage,
                        std::vector<DAEuint32> const&   in_queue_family_indices) noexcept;

        Buffer(Buffer const&    in_copy) = delete;
        Buffer(Buffer&&         in_move) noexcept;

        ~Buffer() noexcept;

        #pragma endregion

        #pragma region Methods

        DAEvoid* Map() noexcept;

        DAEvoid UnMap() noexcept;

        DAEvoid Flush() const noexcept;

        DAEvoid Update(DAEvoid const* in_data, VkDeviceSize in_offset, VkDeviceSize in_size) noexcept;

        [[nodiscard]]
        DAEbool IsMappable() const noexcept;

        [[nodiscard]]
        Device const& GetDevice() const noexcept;

        [[nodiscard]]
        VkBuffer const& GetHandle() const noexcept;

        [[nodiscard]]
        VmaAllocation const& GetAllocation() const noexcept;

        [[nodiscard]]
        VkDeviceMemory const& GetMemory() const noexcept;

        [[nodiscard]]
        VkDeviceSize const& GetOffset() const noexcept;

        [[nodiscard]]
        VkDeviceSize const& GetSize() const noexcept;

        [[nodiscard]]
        DAEvoid* GetMappedData() const noexcept;

        #pragma endregion

        #pragma region Operators

        Buffer& operator=(Buffer const& in_copy) = delete;
        Buffer& operator=(Buffer&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE