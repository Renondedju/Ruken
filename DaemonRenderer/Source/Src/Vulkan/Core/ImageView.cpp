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

#include "Vulkan/Core/ImageView.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

ImageView::ImageView(Image                 const& in_image,
                     VkImageViewCreateInfo const& in_image_view_create_info) noexcept:
    m_image {in_image},
    m_info  {in_image_view_create_info}
{
    VK_CHECK(vkCreateImageView(VulkanLoader::GetLoadedDevice(), &in_image_view_create_info, nullptr, &m_handle));
}

ImageView::ImageView(ImageView&& in_move) noexcept:
    m_image     {in_move.m_image},
    m_handle    {in_move.m_handle},
    m_info      {in_move.m_info}
{
    in_move.m_handle = nullptr;
}

ImageView::~ImageView() noexcept
{
    if (!m_handle)
        return;

    vkDestroyImageView(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

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