#define TINYOBJLOADER_IMPLEMENTATION

#include <tinyobjloader/tiny_obj_loader.h>

#include "Rendering/Resources/Model.hpp"

#include "Core/ServiceProvider.hpp"

#include "Rendering/Resources/Texture.hpp"

#include "Rendering/RenderDevice.hpp"

#include "Rendering/Passes/ForwardPass.hpp"

#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Texture> g_texture;

Model::Model(RenderDevice* in_device, std::string_view in_path) noexcept:
    m_device {in_device}
{
    g_texture = std::make_unique<Texture>(m_device, "Data/viking_room.png");

    tinyobj::ObjReader       reader;
    tinyobj::ObjReaderConfig config;

    reader.ParseFromFile(in_path.data(), config);

    auto& attribute = reader.GetAttrib();
    auto& shapes    = reader.GetShapes();

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

    vk::AllocationCreateInfo allocation_create_info = {
        .usage = vk::MemoryUsage::eGpuOnly
    };

    vk::BufferCreateInfo buffer_create_info = {
        .size  = vertices.size() * sizeof(Vertex) + indices.size() * sizeof(RkUint32),
        .usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
    };

    auto [result, value] = m_device->GetAllocator().createBuffer(buffer_create_info, allocation_create_info, m_allocation_info);

    std::tie(m_buffer, m_allocation) = value;

    m_offset = vertices.size() * sizeof(Vertex);
    m_count  = indices .size();

    buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferSrc;

    allocation_create_info.flags = vk::AllocationCreateFlagBits::eMapped;
    allocation_create_info.usage = vk::MemoryUsage::eCpuOnly;

    vk::AllocationInfo allocation_info;

    std::tie(result, value) = m_device->GetAllocator().createBuffer(buffer_create_info, allocation_create_info, allocation_info);;

    auto [staging_buffer, staging_allocation] = value;

    memcpy(allocation_info.pMappedData,                                  vertices.data(), vertices.size() * sizeof(Vertex));
    memcpy(static_cast<RkByte*>(allocation_info.pMappedData) + m_offset, indices .data(), indices .size() * sizeof(RkUint32));

    vk::BufferCreateInfo uniform_buffer_info = {
        .size  = sizeof(UniformBufferObject),
        .usage = vk::BufferUsageFlagBits::eUniformBuffer,
    };

    std::tie(result, value) = m_device->GetAllocator().createBuffer(uniform_buffer_info, allocation_create_info, allocation_info);

    std::tie(m_uniform_buffer, m_uniform_allocation) = value;

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
        .pSetLayouts        = &ForwardPass::g_descriptor_set_layout
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

    if (auto transfer_command_buffer = m_device->GetTransferQueue().BeginSingleUseCommandBuffer())
    {
        vk::BufferCopy buffer_copy = {
            .size = buffer_create_info.size
        };

        transfer_command_buffer.copyBuffer(staging_buffer, m_buffer, buffer_copy);

        if (m_device->HasDedicatedTransferQueue())
        {
            vk::BufferMemoryBarrier2KHR buffer_memory_barrier = {
                .srcStageMask        = vk::PipelineStageFlagBits2KHR::eTransfer,
                .srcAccessMask       = vk::AccessFlagBits2KHR::eMemoryWrite,
                .dstStageMask        = vk::PipelineStageFlagBits2KHR::eNone,
                .dstAccessMask       = vk::AccessFlagBits2KHR::eNone,
                .srcQueueFamilyIndex = m_device->GetTransferFamilyIndex(),
                .dstQueueFamilyIndex = m_device->GetGraphicsFamilyIndex(),
                .buffer              = m_buffer,
                .size                = VK_WHOLE_SIZE 
            };

            vk::DependencyInfoKHR dependency_info = {
                .bufferMemoryBarrierCount = 1,
                .pBufferMemoryBarriers    = &buffer_memory_barrier
            };

            transfer_command_buffer.pipelineBarrier2KHR(dependency_info);

            m_device->GetTransferQueue().EndSingleUseCommandBuffer(transfer_command_buffer);

            if (auto graphics_command_buffer = m_device->GetGraphicsQueue().BeginSingleUseCommandBuffer())
            {
                buffer_memory_barrier.srcStageMask  = vk::PipelineStageFlagBits2KHR::eNone;
                buffer_memory_barrier.srcAccessMask = vk::AccessFlagBits2KHR::eNone;
                buffer_memory_barrier.dstStageMask  = vk::PipelineStageFlagBits2KHR::eVertexAttributeInput | vk::PipelineStageFlagBits2KHR::eIndexInput;
                buffer_memory_barrier.dstAccessMask = vk::AccessFlagBits2KHR::eMemoryRead;

                graphics_command_buffer.pipelineBarrier2KHR(dependency_info);

                m_device->GetGraphicsQueue().EndSingleUseCommandBuffer(graphics_command_buffer);
            }
        }

        else
        {
            vk::MemoryBarrier2KHR memory_barrier = {
                .srcStageMask  = vk::PipelineStageFlagBits2KHR::eTransfer,
                .srcAccessMask = vk::AccessFlagBits2KHR::eMemoryWrite,
                .dstStageMask  = vk::PipelineStageFlagBits2KHR::eVertexAttributeInput | vk::PipelineStageFlagBits2KHR::eIndexInput,
                .dstAccessMask = vk::AccessFlagBits2KHR::eMemoryRead
            };

            vk::DependencyInfoKHR dependency_info = {
                .memoryBarrierCount = 1,
                .pMemoryBarriers    = &memory_barrier
            };

            transfer_command_buffer.pipelineBarrier2KHR(dependency_info);

            m_device->GetTransferQueue().EndSingleUseCommandBuffer(transfer_command_buffer);
        }
    }

    m_device->GetAllocator().destroyBuffer(staging_buffer, staging_allocation);

    UniformBufferObject ubo = {
        .model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 10.0f)
    };

    ubo.proj[1][1] *= -1;

    memcpy(allocation_info.pMappedData, &ubo, sizeof(UniformBufferObject));
}

Model::~Model() noexcept
{
    g_texture.reset();

    m_device->GetLogicalDevice().destroy(m_descriptor_pool);

    m_device->GetAllocator().destroyBuffer(m_buffer, m_allocation);
    m_device->GetAllocator().destroyBuffer(m_uniform_buffer, m_uniform_allocation);
}

RkVoid Model::Render(vk::CommandBuffer const& in_command_buffer)
{
    in_command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, ForwardPass::g_pipeline_layout, 0, m_descriptor_set, nullptr);

    vk::DeviceSize offset = 0;

    in_command_buffer.bindVertexBuffers(0, m_buffer, offset);

    in_command_buffer.bindIndexBuffer(m_buffer, m_offset, vk::IndexType::eUint32);

    in_command_buffer.drawIndexed(static_cast<RkUint32>(m_count), 1, 0, 0, 0);
}

RkVoid Model::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid Model::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid Model::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}