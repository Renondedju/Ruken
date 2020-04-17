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

#include <fstream>

#include "Vulkan/Resources/Mesh.hpp"

#include "Vulkan/Core/Device.hpp"

#include "Rendering/RenderSystem.hpp"

#include "Resource/ResourceManager.hpp"
#include "Resource/ResourceProcessingFailure.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructors

Mesh::Mesh(std::vector<Vertex> const& in_vertices, std::vector<DAEuint32> const& in_indices):
    m_vertex_count  {static_cast<DAEuint32>(in_vertices.size())},
    m_index_count   {static_cast<DAEuint32>(in_indices .size())}
{
    UploadData(in_vertices, in_indices);
}

#pragma endregion

#pragma region Methods

DAEvoid Mesh::UploadData(std::vector<Vertex>    const& in_vertices,
                         std::vector<DAEuint32> const& in_indices)
{
    auto const& device = GRenderSystem->GetDevice();

    VkDeviceSize const vertex_buffer_size = sizeof(Vertex)    * in_vertices.size();
    VkDeviceSize const index_buffer_size  = sizeof(DAEuint32) * in_indices .size();

    VkBufferCreateInfo      buffer_create_info     = {};
    VmaAllocationCreateInfo allocation_create_info = {};

    // Creates the staging buffers.
    allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    // Creates the staging vertex buffer.
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = vertex_buffer_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    auto const staging_vertex_buffer(device.CreateBuffer(buffer_create_info, allocation_create_info));

    if (!staging_vertex_buffer.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    // Creates the staging index buffer.
    buffer_create_info.size = index_buffer_size;

    auto const staging_index_buffer(device.CreateBuffer(buffer_create_info, allocation_create_info));

    if (!staging_index_buffer.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    // Creates the actual buffers.
    allocation_create_info.flags = 0u;
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    // Creates the vertex buffer.
    buffer_create_info.size  = vertex_buffer_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    auto vertex_buffer = device.CreateBuffer(buffer_create_info, allocation_create_info);

    if (!vertex_buffer.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    // Creates the index buffer.
    buffer_create_info.size  = index_buffer_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

    auto index_buffer = device.CreateBuffer(buffer_create_info, allocation_create_info);

    if (!index_buffer.has_value())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");

    // Copies the data to the staging buffers.
    memcpy(staging_vertex_buffer->GetMappedData(), in_vertices.data(), vertex_buffer_size);
    memcpy(staging_index_buffer ->GetMappedData(), in_indices .data(), index_buffer_size);

    m_vertex_buffer = std::make_unique<Buffer>(std::move(vertex_buffer.value()));
    m_index_buffer  = std::make_unique<Buffer>(std::move(index_buffer .value()));

    // TODO : Uploads the data on the GPU.
}

#pragma warning (disable : 4100)

DAEvoid Mesh::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    auto const& device     = GRenderSystem->GetDevice();
    auto const& descriptor = reinterpret_cast<MeshLoadingDescriptor const&>(in_descriptor);

    if (descriptor.path.empty())
    {
        std::ifstream stream("", std::ios::in | std::ios::binary);

        if (!stream.is_open())
            return;

        std::vector<DAEchar> data;

        stream.read(data.data(), data.size());
    }

    else
    {

    }
}

DAEvoid Mesh::Reload(ResourceManager& in_manager)
{

}

DAEvoid Mesh::Unload(ResourceManager& in_manager) noexcept
{
    std::ofstream stream("", std::ios::out | std::ios::binary);

    if (stream.is_open())
    {
        auto const& device = GRenderSystem->GetDevice();

        VkBufferCreateInfo      buffer_create_info     = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        VmaAllocationCreateInfo allocation_create_info = {};

        // Creates the staging buffers.
        allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        // Creates the staging vertex buffer.
        buffer_create_info.size  = m_vertex_buffer->GetSize();
        buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        auto const staging_vertex_buffer(device.CreateBuffer(buffer_create_info, allocation_create_info));

        if (!staging_vertex_buffer.has_value())
            return;

        // Creates the staging index buffer.
        buffer_create_info.size = m_index_buffer->GetSize();

        auto const staging_index_buffer(device.CreateBuffer(buffer_create_info, allocation_create_info));

        if (!staging_index_buffer.has_value())
            return;

        // TODO : Copies data from GPU.

        try
        {
            stream.write(static_cast<DAEchar const*>(staging_vertex_buffer->GetMappedData()), staging_vertex_buffer->GetSize());
            stream.write(static_cast<DAEchar const*>(staging_vertex_buffer->GetMappedData()), staging_vertex_buffer->GetSize());
        }
        
        catch (std::exception const& e)
        {
            
        }
    }

    m_vertex_buffer.reset();
    m_index_buffer .reset();
}

#pragma warning (default : 4100)

Buffer const& Mesh::GetVertexBuffer() const noexcept
{
    return *m_vertex_buffer;
}

Buffer const& Mesh::GetIndexBuffer() const noexcept
{
    return *m_index_buffer;
}

DAEuint32 Mesh::GetVertexCount() const noexcept
{
    return m_vertex_count;
}

DAEuint32 Mesh::GetIndexCount() const noexcept
{
    return m_index_count;
}

#pragma endregion