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

#include "Rendering/RenderSystem.hpp"

#include "Resource/ResourceProcessingFailure.hpp"

#include "Vulkan/Core/VulkanDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

std::optional<VulkanBuffer> Mesh::CreateStagingBuffer(DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    return GRenderSystem->GetDeviceAllocator().CreateBuffer(buffer_create_info, allocation_create_info);
}

std::optional<VulkanBuffer> Mesh::CreateVertexBuffer(DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    return GRenderSystem->GetDeviceAllocator().CreateBuffer(buffer_create_info, allocation_create_info);
}

std::optional<VulkanBuffer> Mesh::CreateIndexBuffer(DAEuint64 const in_size) noexcept
{
    VmaAllocationCreateInfo allocation_create_info = {};

    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkBufferCreateInfo buffer_create_info = {};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size  = in_size;
    buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    return GRenderSystem->GetDeviceAllocator().CreateBuffer(buffer_create_info, allocation_create_info);
}

DAEvoid Mesh::UploadData(std::vector<Vertex>    const& in_vertices,
                         std::vector<DAEuint32> const& in_indices) const
{
    auto& device = GRenderSystem->GetDevice();
    auto const vertex_buffer_size = sizeof(Vertex)    * in_vertices.size();
    auto const index_buffer_size  = sizeof(DAEuint32) * in_indices .size();

    auto const staging_vertex_buffer = CreateStagingBuffer(vertex_buffer_size);
    auto const staging_index_buffer  = CreateStagingBuffer(index_buffer_size);

    if (!staging_vertex_buffer || !staging_index_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    auto* queue         = device.GetTransferQueue();
    auto command_buffer = device.AllocateCommandBuffer(queue->GetFamilyIndex(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    if (!staging_vertex_buffer || !staging_index_buffer || !queue || !command_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    memcpy(staging_vertex_buffer->GetMappedData(), in_vertices.data(), vertex_buffer_size);
    memcpy(staging_index_buffer ->GetMappedData(), in_indices .data(), index_buffer_size);

    CopyBuffers(*command_buffer, *staging_vertex_buffer, *staging_index_buffer);

    VkSubmitInfo submit_info = {};

    submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1u;
    submit_info.pCommandBuffers    = &command_buffer->GetHandle();

    VulkanFence const fence;

    queue->Submit(submit_info, fence.GetHandle());

    fence.Wait();
}

DAEvoid Mesh::CopyBuffers(VulkanCommandBuffer const& in_command_buffer, VulkanBuffer const& in_vertex_buffer, VulkanBuffer const& in_index_buffer) const noexcept
{
    in_command_buffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferCopy region = {};

    region.size = m_vertex_buffer->GetSize();

    in_command_buffer.CopyBuffer(in_vertex_buffer, *m_vertex_buffer, region);

    region.size = m_index_buffer->GetSize();

    in_command_buffer.CopyBuffer(in_index_buffer, *m_index_buffer, region);

    in_command_buffer.End();
}

#pragma warning (disable : 4100)

DAEvoid Mesh::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<MeshLoadingDescriptor const&>(in_descriptor);

    tinyobj::attrib_t attribute;

    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    if (!LoadObj(&attribute, &shapes, &materials, &warning, &error, m_loading_descriptor.path.c_str()))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    std::vector<Vertex> vertices;
    std::vector<DAEuint32> indices;

    /*std::vector<std::unordered_map<Vertex, DAEuint32>> unique_vertices_per_material(materials.size());

    for (auto const& shape : shapes)
    {
        for (DAEsize i = 0; i < shape.mesh.num_face_vertices.size(); ++i)
        {
            DAEuint8 count       = shape.mesh.num_face_vertices[i];
            DAEint32 material_id = shape.mesh.material_ids     [i];

            auto& unique_vertices = unique_vertices_per_material[material_id + 1];

            std::vector<Vertex>    vertices;
            std::vector<DAEuint32> indices;

            for (DAEuint8 j = 0; j < count; ++j)
            {
                auto const& index = shape.mesh.indices[j];

                Vertex vertex = {
                    Vector3f(attribute.vertices[3 * index.vertex_index + 0],
                             attribute.vertices[3 * index.vertex_index + 1],
                             attribute.vertices[3 * index.vertex_index + 2]),
                    Vector3f(attribute.normals[3 * index.normal_index + 0],
                             attribute.normals[3 * index.normal_index + 1],
                             attribute.normals[3 * index.normal_index + 2]),
                    Vector2f(0.0f + attribute.texcoords[2 * index.texcoord_index + 0],
                             1.0f - attribute.texcoords[2 * index.texcoord_index + 1])
                };

                if (unique_vertices.count(vertex) == 0)
                {
                    unique_vertices[vertex] = static_cast<DAEuint32>(vertices.size());

                    vertices.emplace_back(vertex);
                }

                indices.emplace_back(unique_vertices[vertex]);
            }

            UploadData(vertices, indices);
        }
    }*/

    m_vertex_buffer = std::make_unique<VulkanBuffer>(CreateVertexBuffer(vertices.size()).value());
    m_index_buffer  = std::make_unique<VulkanBuffer>(CreateIndexBuffer (indices .size()).value());

    if (!m_vertex_buffer || !m_index_buffer)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    UploadData(vertices, indices);

    VulkanDebug::SetObjectName(VK_OBJECT_TYPE_BUFFER, reinterpret_cast<DAEuint64>(m_vertex_buffer->GetHandle()), "");
    VulkanDebug::SetObjectName(VK_OBJECT_TYPE_BUFFER, reinterpret_cast<DAEuint64>(m_index_buffer ->GetHandle()), "");
}

DAEvoid Mesh::Reload(ResourceManager& in_manager)
{
    tinyobj::attrib_t attribute;

    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    if (!LoadObj(&attribute, &shapes, &materials, &warning, &error, m_loading_descriptor.path.c_str()))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    std::vector<Vertex> vertices;
    std::vector<DAEuint32> indices;



    UploadData(vertices, indices);
}

DAEvoid Mesh::Unload(ResourceManager& in_manager) noexcept
{
    m_vertex_buffer.reset();
    m_index_buffer .reset();
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