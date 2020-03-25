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

#include "Vulkan/BufferView.hpp"
#include "Vulkan/Buffer.hpp"
#include "Vulkan/Device.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

BufferView::BufferView(Device*                          in_device,
                       Buffer*                          in_buffer,
                       VkBufferView                     in_handle,
                       VkBufferViewCreateInfo const&    in_info) noexcept :
    m_device    { in_device },
    m_buffer    { in_buffer },
    m_handle    { in_handle },
    m_info      { in_info }
{
    
}

BufferView::~BufferView()
{
    if (m_handle)
    {
        vkDestroyBufferView(m_device->GetHandle(), m_handle, nullptr);
    }
}

#pragma endregion

#pragma region Methods

DAEbool BufferView::Create(Device*                          in_device,
                           Buffer*                          in_buffer,
                           VkBufferViewCreateInfo const&    in_create_info,
                           std::unique_ptr<BufferView>&     out_buffer_view) noexcept
{
    VkBufferView buffer_view;

    auto const result = vkCreateBufferView(in_device->GetHandle(), &in_create_info, nullptr, &buffer_view);

    if (result == VK_SUCCESS)
    {
        out_buffer_view = std::make_unique<BufferView>(in_device, in_buffer, buffer_view, in_create_info);
    }

    return result == VK_SUCCESS;
}

VkBufferView BufferView::GetHandle() const noexcept
{
    return m_handle;
}

VkFormat BufferView::GetFormat() const noexcept
{
    return m_info.format;
}

VkDeviceSize BufferView::GetOffset() const noexcept
{
    return m_info.offset;
}

VkDeviceSize BufferView::GetRange() const noexcept
{
    return m_info.range;
}

#pragma endregion