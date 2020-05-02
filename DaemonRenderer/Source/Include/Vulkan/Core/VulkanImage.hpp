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
 * \brief RAII-class wrapping a 'VkImage' object.
 *        Images represent multidimensional - up to 3 - arrays of data which can be used for various purposes(e.g. attachments, textures),
 *        by binding them to a graphics or compute pipeline via descriptor sets,
 *        or by directly specifying them as parameters to certain commands.
 * \note  An image must be allocated from the DeviceMemoryAllocator and the memory will be freed at object's destruction.
 */
class VulkanImage
{
    private:

        #pragma region Members

        VkImage       m_handle     {nullptr};
        VmaAllocator  m_allocator  {nullptr};
        VmaAllocation m_allocation {nullptr};
        VkImageType   m_type       {};
        VkFormat      m_format     {VK_FORMAT_UNDEFINED};
        VkExtent3D    m_extent     {};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanImage(VkImage    in_handle,
                             VkFormat   in_format,
                             VkExtent3D in_extent) noexcept;

        explicit VulkanImage(VkImage       in_handle,
                             VmaAllocator  in_allocator,
                             VmaAllocation in_allocation,
                             VkFormat      in_format,
                             VkExtent3D    in_extent) noexcept;

        VulkanImage(VulkanImage const& in_copy) = delete;
        VulkanImage(VulkanImage&&      in_move) noexcept;

        ~VulkanImage() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] VkImage     const& GetHandle() const noexcept;
        [[nodiscard]] VkImageType const& GetType  () const noexcept;
        [[nodiscard]] VkFormat    const& GetFormat() const noexcept;
        [[nodiscard]] VkExtent3D  const& GetExtent() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanImage& operator=(VulkanImage const& in_copy) = delete;
        VulkanImage& operator=(VulkanImage&&      in_move) noexcept;

        #pragma endregion
};

DAEbool operator==(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept;
DAEbool operator!=(VkExtent3D const& in_lhs, VkExtent3D const& in_rhs) noexcept;

END_DAEMON_NAMESPACE