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

class Image;
class Device;

class ImageView
{
    private:

        #pragma region Members

        Device*                 m_device;
        Image*                  m_image;
        VkImageView             m_handle;
        VkImageViewCreateInfo   m_info;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        ImageView() = delete;

        explicit ImageView(Device*                          in_device,
                           Image*                           in_image,
                           VkImageView                      in_handle,
                           VkImageViewCreateInfo const&     in_info) noexcept;

        ImageView(ImageView const&  in_copy) noexcept = delete;
        ImageView(ImageView&&       in_move) noexcept = default;

        ~ImageView() noexcept;

        #pragma endregion

        #pragma region Operators

        ImageView& operator=(ImageView const&   in_copy) noexcept = delete;
        ImageView& operator=(ImageView&&        in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        static DAEbool Create(Device*                       in_device,
                              Image*                        in_image,
                              VkImageViewCreateInfo const&  in_create_info,
                              ImageView**                   out_image_view) noexcept;

        static DAEvoid Destroy(ImageView* in_image_view);

        [[nodiscard]] VkImageView GetHandle() const noexcept;

        [[nodiscard]] VkImageViewType GetViewType() const noexcept;

        [[nodiscard]] VkFormat GetFormat() const noexcept;

        [[nodiscard]] VkComponentMapping GetComponents() const noexcept;

        [[nodiscard]] VkImageSubresourceRange GetSubresourceRange() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE