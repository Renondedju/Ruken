#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"
#include "Rendering/Renderer.hpp"

#include "Core/ServiceProvider.hpp"

#include "Windowing/WindowManager.hpp"

#include "Rendering/Resources/Model.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model>    g_model;
static std::unique_ptr<Texture>  g_texture;
static std::unique_ptr<Material> g_material;

RenderSystem::RenderSystem(ServiceProvider& in_service_provider) noexcept:
    m_device {in_service_provider.LocateService<Renderer>     ()->GetDevice    ()},
    m_window {in_service_provider.LocateService<WindowManager>()->GetMainWindow()}
{
    m_graph = std::make_unique<RenderGraph>(nullptr, m_device);

    auto& forward_pass = m_graph->AddRenderPass("forward");

    forward_pass.SetCallback([&](vk::CommandBuffer const& in_command_buffer) {

        m_frames[m_current_frame]->Bind(in_command_buffer);

        g_material->Bind  (in_command_buffer);
        g_model   ->Render(in_command_buffer);
    });

    g_model    = std::make_unique<Model>   (m_device, "Data/viking_room.obj");
    g_texture  = std::make_unique<Texture> (m_device, "Data/viking_room.png");
    g_material = std::make_unique<Material>(m_device, "");

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_frames.emplace_back(std::make_unique<RenderFrame>(nullptr, m_device));
    }
}

RenderSystem::~RenderSystem() noexcept
{
    if (m_device->GetLogicalDevice().waitIdle() != vk::Result::eSuccess)
    {
        
    }

    g_material.reset();
    g_texture .reset();
    g_model   .reset();
    m_graph   .reset();
}

RkVoid RenderSystem::Update() noexcept
{
    m_current_frame = (m_current_frame + 1) % 2;

    auto& frame = *m_frames[m_current_frame];

    frame.Reset();

    auto& draw_buffer      = frame.GetDrawStorageBuffer     ();
    auto& transform_buffer = frame.GetTransformStorageBuffer();
    auto& material_buffer  = frame.GetMaterialStorageBuffer ();

    draw_buffer     .Resize(sizeof(DrawData),      vk::BufferUsageFlagBits::eStorageBuffer);
    transform_buffer.Resize(sizeof(TransformData), vk::BufferUsageFlagBits::eStorageBuffer);
    material_buffer .Resize(sizeof(MaterialData),  vk::BufferUsageFlagBits::eStorageBuffer);

    DrawData draw_data = {
        .material_index  = 0U,
        .transform_index = 0U
    };

    TransformData transform_data = {
        .model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f))
    };

    MaterialData material_data = {
        .albedo_texture = 0U,
        .normal_texture = 1U
    };

    memcpy(draw_buffer.Map(), &draw_data, sizeof(DrawData));
    draw_buffer.UnMap();

    memcpy(transform_buffer.Map(), &transform_data, sizeof(TransformData));
    transform_buffer.UnMap();

    memcpy(material_buffer.Map(), &material_data, sizeof(MaterialData));
    material_buffer.UnMap();

    vk::DescriptorBufferInfo draw_descriptor_buffer_info = {
        .buffer = draw_buffer.GetHandle(),
        .offset = 0ULL,
        .range  = sizeof(DrawData)
    };

    vk::DescriptorBufferInfo transform_descriptor_buffer_info = {
        .buffer = transform_buffer.GetHandle(),
        .offset = 0ULL,
        .range  = sizeof(TransformData)
    };

    vk::DescriptorBufferInfo material_descriptor_buffer_info = {
        .buffer = material_buffer.GetHandle(),
        .offset = 0ULL,
        .range  = sizeof(MaterialData)
    };

    vk::DescriptorImageInfo texture_descriptor_image_info = {
        .sampler     = g_texture->GetImageSampler(),
        .imageView   = g_texture->GetImageView(),
        .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
    };

    std::array<vk::WriteDescriptorSet, 4> write_descriptors = {
        {
            {
                .dstSet = frame.GetFrameDescriptorSet(),
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eStorageBuffer,
                .pBufferInfo = &draw_descriptor_buffer_info
            },
            {
                .dstSet = frame.GetFrameDescriptorSet(),
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eStorageBuffer,
                .pBufferInfo = &transform_descriptor_buffer_info
            },
            {
                .dstSet = frame.GetFrameDescriptorSet(),
                .dstBinding = 2,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eStorageBuffer,
                .pBufferInfo = &material_descriptor_buffer_info
            },
            {
                .dstSet = m_device->GetTextureDescriptorSet(),
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .pImageInfo = &texture_descriptor_image_info
            }
        }
    };

    m_device->GetLogicalDevice().updateDescriptorSets(write_descriptors, VK_NULL_HANDLE);

    m_graph ->Execute(frame);
    m_window->Present(frame);
}