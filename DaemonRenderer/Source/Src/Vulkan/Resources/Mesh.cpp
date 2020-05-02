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

#define TINYOBJLOADER_IMPLEMENTATION

#include <tinyobjloader/tiny_obj_loader.h>

#pragma warning (pop)

#include "Vulkan/Resources/Mesh.hpp"

#include "Rendering/Renderer.hpp"

#include "Resource/ResourceProcessingFailure.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

std::optional<VulkanBuffer> Mesh::CreateStagingBuffer(VulkanDeviceAllocator const& in_allocator, DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    return in_allocator.CreateBuffer(buffer_create_info, allocation_create_info);
}

std::optional<VulkanBuffer> Mesh::CreateVertexBuffer(VulkanDeviceAllocator const& in_allocator, DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    return in_allocator.CreateBuffer(buffer_create_info, allocation_create_info);
}

std::optional<VulkanBuffer> Mesh::CreateIndexBuffer(VulkanDeviceAllocator const& in_allocator, DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    return in_allocator.CreateBuffer(buffer_create_info, allocation_create_info);
}

DAEvoid Mesh::UploadData(VulkanDevice           const& in_device,
                         VulkanDeviceAllocator  const& in_allocator,
                         std::vector<Vertex>    const& in_vertices,
                         std::vector<DAEuint32> const& in_indices) const
{
    auto const vertex_buffer_size = sizeof(Vertex)    * in_vertices.size();
    auto const index_buffer_size  = sizeof(DAEuint32) * in_indices .size();

    auto staging_vertex_buffer = CreateStagingBuffer(in_allocator, vertex_buffer_size);
    auto staging_index_buffer  = CreateStagingBuffer(in_allocator, index_buffer_size);

    if (!staging_vertex_buffer || !staging_index_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "Failed to allocate staging buffers!");

    auto const command_buffer = in_device.GetTransferCommandPool().AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    if (!command_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "Failed to allocate command buffer!");

    if (!staging_vertex_buffer->Update(in_vertices.data(), in_vertices.size()) ||
        !staging_index_buffer ->Update(in_indices .data(), in_indices .size()))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "");

    VulkanFence const fence;

    if (!command_buffer->Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "Failed to begin command buffer!");

    VkBufferCopy region = {};

    region.size = m_vertex_buffer->GetSize();

    command_buffer->CopyBufferToBuffer(*staging_vertex_buffer, *m_vertex_buffer, region);

    region.size = m_index_buffer->GetSize();

    command_buffer->CopyBufferToBuffer(*staging_index_buffer, *m_index_buffer, region);

    if (!command_buffer->End())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "Failed to end command buffer!");

    in_device.GetTransferQueue().Submit(*command_buffer, fence.GetHandle());

    fence.Wait();
}

#pragma warning (disable : 4100)

DAEvoid Mesh::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<MeshLoadingDescriptor const&>(in_descriptor);

    auto const& device    = m_loading_descriptor->renderer.get().GetDevice();
    auto const& allocator = m_loading_descriptor->renderer.get().GetDeviceAllocator();

    tinyobj::attrib_t attribute;

    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    if (!LoadObj(&attribute, &shapes, &materials, &warning, &error, m_loading_descriptor->path))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::CorruptedResource, "Failed to load the .obj file!");

    std::vector<Vertex>    vertices;
    std::vector<DAEuint32> indices;

    // TODO : Load vertices and indices data.

    m_vertex_buffer = CreateVertexBuffer(allocator, sizeof(Vertex)    * vertices.size());
    m_index_buffer  = CreateIndexBuffer (allocator, sizeof(DAEuint32) * indices .size());

    if (!m_vertex_buffer || !m_index_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::OutOfMemory, "Failed to allocate the buffers!");

    UploadData(device, allocator, vertices, indices);

    VulkanDebug::SetObjectName(VK_OBJECT_TYPE_BUFFER, reinterpret_cast<DAEuint64>(m_vertex_buffer->GetHandle()), "");
    VulkanDebug::SetObjectName(VK_OBJECT_TYPE_BUFFER, reinterpret_cast<DAEuint64>(m_index_buffer ->GetHandle()), "");
}

DAEvoid Mesh::Reload(ResourceManager& in_manager)
{
    auto const& device    = m_loading_descriptor->renderer.get().GetDevice();
    auto const& allocator = m_loading_descriptor->renderer.get().GetDeviceAllocator();

    tinyobj::attrib_t attribute;

    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    if (!LoadObj(&attribute, &shapes, &materials, &warning, &error, m_loading_descriptor->path))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::CorruptedResource, "Failed to load the .obj file!");

    std::vector<Vertex>    vertices;
    std::vector<DAEuint32> indices;

    // TODO : Load vertices and indices data.

    if (m_vertex_buffer->GetSize() != sizeof(Vertex) * vertices.size())
        m_vertex_buffer = CreateVertexBuffer(allocator, sizeof(Vertex) * vertices.size());

    if (m_index_buffer->GetSize() != sizeof(DAEuint32) * indices.size())
        m_index_buffer = CreateVertexBuffer(allocator, sizeof(DAEuint32) * indices.size());

    UploadData(device, allocator, vertices, indices);
}

DAEvoid Mesh::Unload(ResourceManager& in_manager) noexcept
{
    m_loading_descriptor.reset();
    m_vertex_buffer     .reset();
    m_index_buffer      .reset();
}

#pragma warning (default : 4100)

VulkanBuffer const& Mesh::GetVertexBuffer() const noexcept
{
    return *m_vertex_buffer;
}

VulkanBuffer const& Mesh::GetIndexBuffer() const noexcept
{
    return *m_index_buffer;
}

#pragma endregion