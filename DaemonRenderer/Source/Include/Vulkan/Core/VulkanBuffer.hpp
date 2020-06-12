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
 * \brief RAII-class wrapping a 'VkBuffer' object.
 *        Buffers represent linear arrays of data which are used for various purposes
 *        by binding them to a graphics or compute pipeline via descriptor sets or via certain commands,
 *        or by directly specifying them as parameters to certain commands.
 * \note  A buffer must be allocated from the VulkanDeviceAllocator and the memory will be freed at buffer's destruction.
 */
class VulkanBuffer
{
    private:

        #pragma region Members

        VkBuffer          m_handle          {nullptr};
        VmaAllocator      m_allocator       {nullptr};
        VmaAllocation     m_allocation      {nullptr};
        VmaAllocationInfo m_allocation_info {};
        DAEbool           m_is_persistent   {false};
        DAEbool           m_is_mapped       {false};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanBuffer(VkBuffer          in_handle,
                              VmaAllocator      in_allocator,
                              VmaAllocation     in_allocation,
                              VmaAllocationInfo in_allocation_info,
                              DAEbool           in_persistent) noexcept;

        VulkanBuffer(VulkanBuffer const& in_copy) = delete;
        VulkanBuffer(VulkanBuffer&&      in_move) noexcept;

        ~VulkanBuffer() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return A pointer to the first byte of this memory if the memory was correctly mapped, else nullptr.
         * \note   This function automatically flushes or invalidates caches if necessary.
         */
        [[nodiscard]]
        DAEvoid* Map() noexcept;

        /**
         * \return True if the data was correctly updated, else False.
         * \note   This function automatically maps and unmaps memory if necessary.
         */
        [[nodiscard]]
        DAEbool Update(DAEvoid const* in_data, DAEsize in_size) noexcept;

        /**
         * \return True if the memory was correctly unmapped, else False.
         * \note   This function automatically flushes or invalidates caches if necessary.
         */
        [[nodiscard]]
        DAEbool Unmap() noexcept;

        [[nodiscard]] VkBuffer       const& GetHandle() const noexcept;
        [[nodiscard]] VkDeviceMemory const& GetMemory() const noexcept;
        [[nodiscard]] VkDeviceSize   const& GetOffset() const noexcept;
        [[nodiscard]] VkDeviceSize   const& GetSize  () const noexcept;
        [[nodiscard]] DAEbool               IsMapped () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanBuffer& operator=(VulkanBuffer const& in_copy) = delete;
        VulkanBuffer& operator=(VulkanBuffer&&      in_move) noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE