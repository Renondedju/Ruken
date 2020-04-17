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

#pragma warning (push, 0)

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#pragma warning (pop)

#include "Vulkan/Resources/Texture.hpp"

#include "Vulkan/Core/Device.hpp"

#include "Resource/ResourceManager.hpp"

#include "Rendering/RenderSystem.hpp"
#include "Resource/ResourceProcessingFailure.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor

Texture::Texture(std::string const& in_filename)
{
    DAEint32 width  = 0;
    DAEint32 height = 0;
    DAEint32 comp   = 0;

    auto* pixels = stbi_load(in_filename.c_str(), &width, &height, &comp, 0);

    UploadData(pixels, width, height);
}

Texture::Texture(Texture&& in_move) noexcept:
    m_image {std::move(in_move.m_image)}
{

}

#pragma endregion

#pragma region Methods

DAEvoid Texture::UploadData(DAEvoid   const*    in_pixels,
                            DAEuint32 const     in_width,
                            DAEuint32 const     in_height)
{
    auto const& device = GRenderSystem->GetDevice();

    VmaAllocationCreateInfo allocation_create_info = {};
    VkBufferCreateInfo      buffer_create_info     = {};
    VkImageCreateInfo       image_create_info      = {};

    // Creates the staging buffer.
    allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_width * in_height * sizeof(DAEuint32);
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    auto staging_buffer = device.CreateBuffer(buffer_create_info, allocation_create_info);

    if (!staging_buffer.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    memcpy(staging_buffer->GetMappedData(), in_pixels, in_width * in_height * sizeof(DAEuint32));

    // Creates the image.
    allocation_create_info.flags = 0u;
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType     = VK_IMAGE_TYPE_2D;
    image_create_info.format        = VK_FORMAT_R8G8B8A8_UNORM;
    image_create_info.extent.width  = in_width;
    image_create_info.extent.height = in_height;
    image_create_info.extent.depth  = 1u;
    image_create_info.mipLevels     = 1u;
    image_create_info.arrayLayers   = 1u;
    image_create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage         = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    auto image = device.CreateImage(image_create_info, allocation_create_info);

    if (!image.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    m_image = std::make_unique<Image>(std::move(image.value()));

    // TODO : Upload data to GPU.
}

#pragma warning (disable : 4100)

DAEvoid Texture::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    auto const& descriptor = reinterpret_cast<TextureLoadingDescriptor const&>(in_descriptor);

    DAEint32 width  = 0;
    DAEint32 height = 0;
    DAEint32 comp   = 0;

    auto* pixels = stbi_load(descriptor.path.c_str(), &width, &height, &comp, 0);

    UploadData(pixels, width, height);
}

DAEvoid Texture::Reload(ResourceManager& in_manager)
{

}

DAEvoid Texture::Unload(ResourceManager& in_manager) noexcept
{
    m_image.reset();
}

#pragma warning (default : 4100)

Image const& Texture::GetImage() const noexcept
{
    return *m_image;
}

#pragma endregion