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

class Image
{
    private:

        #pragma region Members

        VkImage         m_handle;
        VmaAllocation   m_allocation;
        VkFormat        m_format;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Image() = delete;

        explicit Image(VkImage              in_handle,
                       VmaAllocation        in_allocation) noexcept;

        Image(Image const&  in_copy) noexcept = delete;
        Image(Image&&       in_move) noexcept = default;

        ~Image() = default;

        #pragma endregion

        #pragma region Operators

        Image& operator=(Image const&   in_copy) noexcept = delete;
        Image& operator=(Image&&        in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] VkImage GetHandle() const noexcept;

        [[nodiscard]] VmaAllocation GetAllocation() const noexcept;

        [[nodiscard]] VkFormat GetFormat() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE