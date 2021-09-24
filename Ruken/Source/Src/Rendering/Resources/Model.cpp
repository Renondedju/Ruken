#define TINYOBJLOADER_IMPLEMENTATION

#include <tinyobjloader/tiny_obj_loader.h>

#include "Rendering/Resources/Model.hpp"
#include "Rendering/Resources/Texture.hpp"

#include "Rendering/Renderer.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Texture> g_texture;

Model::Model(RenderDevice* in_device, std::string_view in_path) noexcept: DeviceObjectBase(in_device)
{
    tinyobj::attrib_t                attribute;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    if (!LoadObj(&attribute, &shapes, &materials, &warn, &err, in_path.data()))
        return;

    std::vector<Vertex>   vertices;
    std::vector<RkUint32> indices;

    for (auto const& [name, mesh, lines, points] : shapes)
    {
        for (auto const& [vertex_index, normal_index, uv_index] : mesh.indices)
        {
            Vertex vertex = {
                .position = {
                    attribute.vertices[3 * vertex_index + 0],
                    attribute.vertices[3 * vertex_index + 1],
                    attribute.vertices[3 * vertex_index + 2]
                },
                .color = {
                    1.0F,
                    1.0F,
                    1.0F
                },
                .uv = {
                    attribute.texcoords[2 * uv_index + 0],
                    1.0F - attribute.texcoords[2 * uv_index + 1]
                }
            };

            vertices.push_back(vertex);
            indices .push_back(static_cast<RkUint32>(indices.size()));
        }
    }

    vk::AllocationCreateInfo allocation_info = {
        .flags = vk::AllocationCreateFlagBits::eMapped,
        .usage = vk::MemoryUsage::eCpuToGpu
    };

    vk::BufferCreateInfo buffer_info = {
        .size  = vertices.size() * sizeof(Vertex) + indices.size() * sizeof(RkUint32),
        .usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer,
    };

    auto [result, value] = m_device->GetAllocator().createBuffer(buffer_info, allocation_info, m_allocation_info);

    std::tie(m_buffer, m_allocation) = value;

    m_offset = vertices.size() * sizeof(Vertex);
    m_count  = indices .size();

    memcpy(m_allocation_info.pMappedData,            vertices.data(), vertices.size() * sizeof(Vertex));
    // memcpy(m_allocation_info.pMappedData + m_offset, indices .data(), indices .size() * sizeof(RkUint32));

    vk::BufferCreateInfo uniform_buffer_info = {
        .size = sizeof(UniformBufferObject),
        .usage = vk::BufferUsageFlagBits::eUniformBuffer,
    };

    std::tie(result, value) =  m_device->GetAllocator().createBuffer(uniform_buffer_info, allocation_info);

    std::tie(m_uniform_buffer, m_uniform_allocation) = value;

    g_texture = std::make_unique<Texture>(m_device, "Data/viking_room.png");

    vk::DescriptorPoolSize pool_sizes[2] = {
        {
            .type            = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1U
        },
        {
            .type            = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 1U
        }
    };

    vk::DescriptorPoolCreateInfo descriptor_pool_info = {
        .maxSets = 1U,
        .poolSizeCount = 2U,
        .pPoolSizes = pool_sizes
    };

    m_descriptor_pool = m_device->GetLogicalDevice().createDescriptorPool(descriptor_pool_info).value;

    vk::DescriptorSetAllocateInfo descriptor_set_allocate_info = {
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &Renderer::descriptor_set_layout
    };

    m_descriptor_set = m_device->GetLogicalDevice().allocateDescriptorSets(descriptor_set_allocate_info).value.front();

    vk::DescriptorBufferInfo descriptor_buffer_info = {
        .buffer = m_uniform_buffer,
        .offset = 0,
        .range = sizeof(UniformBufferObject)
    };

    vk::DescriptorImageInfo descriptor_image_info = {
        .sampler     = g_texture->GetImageSampler(),
        .imageView   = g_texture->GetImageView(),
        .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
    };

    std::array<vk::WriteDescriptorSet, 2> write_descriptors{
        {
            {
                .dstSet = m_descriptor_set,
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .pBufferInfo = &descriptor_buffer_info
            },
            {
                .dstSet = m_descriptor_set,
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .pImageInfo = &descriptor_image_info
            }
        }
    };

    m_device->GetLogicalDevice().updateDescriptorSets(write_descriptors, VK_NULL_HANDLE);
}

Model::~Model() noexcept
{
    g_texture.reset();

    m_device->GetAllocator().destroyBuffer(m_buffer, m_allocation);
    m_device->GetAllocator().destroyBuffer(m_uniform_buffer, m_uniform_allocation);
}

RkVoid Model::Render(vk::CommandBuffer const& in_command_buffer)
{
    in_command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, Renderer::pipeline_layout, 0, m_descriptor_set, nullptr);

    vk::DeviceSize offset = 0;

    in_command_buffer.bindVertexBuffers(0, m_buffer, offset);

    in_command_buffer.bindIndexBuffer(m_buffer, m_offset, vk::IndexType::eUint32);

    in_command_buffer.drawIndexed(static_cast<RkUint32>(m_count), 1, 0, 0, 0);
}