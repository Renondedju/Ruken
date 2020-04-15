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

#include "Vulkan/Core/Image.hpp"

BEGIN_DAEMON_NAMESPACE

class ImageView
{
    private:

        #pragma region Members

        Image const& m_image;

        VkImageView             m_handle    {nullptr};
        VkImageViewCreateInfo   m_info      {};

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit ImageView(Image                 const& in_image,
                           VkImageViewCreateInfo const& in_image_view_create_info) noexcept;

        ImageView(ImageView const&  in_copy) = delete;
        ImageView(ImageView&&       in_move) noexcept;

        ~ImageView() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VkImageView GetHandle() const noexcept;

        [[nodiscard]]
        VkImageViewType GetViewType() const noexcept;

        [[nodiscard]]
        VkFormat GetFormat() const noexcept;

        [[nodiscard]]
        VkComponentMapping GetComponents() const noexcept;

        [[nodiscard]]
        VkImageSubresourceRange GetSubresourceRange() const noexcept;

        #pragma endregion

        #pragma region Operators

        ImageView& operator=(ImageView const&   in_copy) = delete;
        ImageView& operator=(ImageView&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE