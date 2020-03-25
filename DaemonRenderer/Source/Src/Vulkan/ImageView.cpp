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

#include "Vulkan/ImageView.hpp"
#include "Vulkan/Image.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

ImageView::ImageView(Device*                        in_device,
                     Image*                         in_image,
                     VkImageView                    in_handle,
                     VkImageViewCreateInfo const&   in_info) noexcept :
    m_device    { in_device },
    m_image     { in_image },
    m_handle    { in_handle },
    m_info      { in_info }
{

}

ImageView::~ImageView() noexcept
{
    if (m_handle)
    {
        vkDestroyImageView(m_device->GetHandle(), m_handle, nullptr);
    }
}

#pragma endregion

#pragma region Methods

DAEbool ImageView::Create(Device*                       in_device,
                          Image*                        in_image,
                          VkImageViewCreateInfo const&  in_create_info,
                          ImageView**                   out_image_view) noexcept
{
    VkImageView image_view = nullptr;

    auto const result = vkCreateImageView(in_device->GetHandle(), &in_create_info, nullptr, &image_view);

    *out_image_view = new ImageView(in_device, in_image, image_view, in_create_info);

    return result == VK_SUCCESS;
}

DAEvoid ImageView::Destroy(ImageView* in_image_view)
{
    delete in_image_view;
}

VkImageView ImageView::GetHandle() const noexcept
{
    return m_handle;
}

VkImageViewType ImageView::GetViewType() const noexcept
{
    return m_info.viewType;
}

VkFormat ImageView::GetFormat() const noexcept
{
    return m_info.format;
}

VkComponentMapping ImageView::GetComponents() const noexcept
{
    return m_info.components;
}

VkImageSubresourceRange ImageView::GetSubresourceRange() const noexcept
{
    return m_info.subresourceRange;
}

#pragma endregion