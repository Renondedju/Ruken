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

#include "Vulkan/Core/BufferView.hpp"

#include "Vulkan/Utilities/Debug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors and Destructor

BufferView::BufferView(Buffer                 const& in_buffer,
                       VkBufferViewCreateInfo const& in_buffer_view_create_info) noexcept:
    m_buffer    {in_buffer},
    m_info      {in_buffer_view_create_info}
{
    VK_CHECK(vkCreateBufferView(Loader::GetLoadedDevice(), &in_buffer_view_create_info, nullptr, &m_handle));
}

BufferView::BufferView(BufferView&& in_move) noexcept:
    m_buffer    {in_move.m_buffer},
    m_handle    {in_move.m_handle},
    m_info      {in_move.m_info}
{
    in_move.m_handle = nullptr;
}

BufferView::~BufferView() noexcept
{
    if (!m_handle)
        return;

    vkDestroyBufferView(Loader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

VkBufferView const& BufferView::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion