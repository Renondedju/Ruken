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
    m_renderer {in_service_provider.LocateService<Renderer>     ()},
    m_window   {in_service_provider.LocateService<WindowManager>()->GetMainWindow()}
{
    auto& forward_pass = m_renderer->GetGraph()->FindOrAddRenderPass("Forward");

    forward_pass.AddColorOutput("SceneColor");

    forward_pass.SetCallback([&](vk::CommandBuffer const& in_command_buffer, RenderFrame const& in_frame) {
        g_material->Bind  (in_command_buffer);
        g_model   ->Render(in_command_buffer);
    });

    auto& final_pass = m_renderer->GetGraph()->FindOrAddRenderPass("Final");

    final_pass.SetCallback([&](vk::CommandBuffer const& in_command_buffer, RenderFrame const& in_frame) {

    });

    g_model    = std::make_unique<Model>   (m_renderer, "Data/viking_room.obj");
    g_texture  = std::make_unique<Texture> (m_renderer, "Data/viking_room.png");
    g_material = std::make_unique<Material>(m_renderer, "");

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_frames.emplace_back(std::make_unique<RenderFrame>(nullptr, m_renderer->GetDevice(), i));
    }
}

RenderSystem::~RenderSystem() noexcept
{
    if (m_renderer->GetDevice()->GetLogicalDevice().waitIdle() != vk::Result::eSuccess)
    {
        
    }

    g_material.reset();
    g_texture .reset();
    g_model   .reset();
}

RkVoid RenderSystem::Update() noexcept
{
    m_current_frame = (m_current_frame + 1U) % 2U;

    auto& frame = *m_frames[m_current_frame];

    frame.Reset();

    // Frame data.
    auto& draw_buffer      = frame.GetDrawStorageBuffer     ();
    auto& transform_buffer = frame.GetTransformStorageBuffer();
    auto& material_buffer  = frame.GetMaterialStorageBuffer ();

    draw_buffer     .Resize(sizeof(DrawData));
    transform_buffer.Resize(sizeof(TransformData));
    material_buffer .Resize(sizeof(MaterialData));

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

    std::array<vk::WriteDescriptorSet, 3> write_descriptors = {
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
            }
        }
    };

    m_renderer->GetDevice()->GetLogicalDevice().updateDescriptorSets(write_descriptors, VK_NULL_HANDLE);

    // foreach camera
    {
        // Camera data.
        auto const& camera_buffer = frame.GetCameraUniformBuffer();

        CameraData ubo = {
            .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            .proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 10.0f)
        };

        ubo.proj[1][1] *= -1;

        memcpy(camera_buffer.Map(), &ubo, sizeof(CameraData));

        camera_buffer.UnMap();

        RkUint32 const image_index = m_window->AcquireNextImage(nullptr);

        // Execute graph.
        m_renderer->GetGraph()->Execute(frame);

        // if renderToScreen
        m_window->Present(nullptr, image_index);
    }
}