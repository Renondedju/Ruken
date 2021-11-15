#include "Rendering/RenderSystem.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderGraph.hpp"
#include "Rendering/Renderer.hpp"

#include "Core/ServiceProvider.hpp"

#include "Windowing/WindowManager.hpp"

#include "Rendering/Resources/Model.hpp"
#include "Rendering/Resources/MaterialInstance.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model>            g_model;
static std::unique_ptr<Texture>          g_texture;
static std::unique_ptr<Material>         g_material;
static std::unique_ptr<MaterialInstance> g_material_instance;

RenderSystem::RenderSystem(ServiceProvider& in_service_provider) noexcept:
    m_renderer {in_service_provider.LocateService<Renderer>     ()},
    m_window   {in_service_provider.LocateService<WindowManager>()->GetMainWindow()}
{
    auto& forward_pass = m_renderer->GetGraph()->AddPass("Forward");

    ImageInfo info = {
        .format = vk::Format::eR8G8B8A8Unorm,
        .extent = {
            .width  = 1920,
            .height = 1080,
            .depth  = 1
        },
        .usage  = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc
    };

    forward_pass.AddColorOutput("Final", info);

    info.format = vk::Format::eD32Sfloat;
    info.usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    forward_pass.SetDepthStencilOutput("Depth", info);

    forward_pass.SetCallback([&](vk::CommandBuffer const& in_command_buffer) {
        RkUint32 index = 0U;

        in_command_buffer.pushConstants(m_renderer->GetGraph()->GetDefaultPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0U, sizeof index, &index);

        g_material->Bind  (in_command_buffer);
        g_model   ->Render(in_command_buffer);
    });

    m_renderer->GetGraph()->Build();

    g_model             = std::make_unique<Model>           (m_renderer, "Data/viking_room.obj");
    g_texture           = std::make_unique<Texture>         (m_renderer, "Data/viking_room.png");
    g_material          = std::make_unique<Material>        (m_renderer, "");
    g_material_instance = std::make_unique<MaterialInstance>(m_renderer, g_material.get());
}

RenderSystem::~RenderSystem() noexcept
{
    if (m_renderer->GetDevice()->GetLogicalDevice().waitIdle() != vk::Result::eSuccess)
    {
        
    }

    g_material_instance.reset();
    g_material         .reset();
    g_texture          .reset();
    g_model            .reset();
}

RkVoid RenderSystem::Update() noexcept
{
    auto& frame = m_renderer->GetGraph()->BeginFrame();

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

    draw_buffer     .Upload(&draw_data);
    transform_buffer.Upload(&transform_data);
    material_buffer .Upload(&material_data);

    vk::DescriptorImageInfo texture_descriptor_image_info = {
        .sampler     = g_texture->GetSampler(),
        .imageView   = g_texture->GetView(),
        .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
    };

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

    std::array<vk::WriteDescriptorSet, 4> write_descriptors = {
        {
            {
                .dstSet = frame.GetTextureDescriptorSet(),
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .pImageInfo = &texture_descriptor_image_info
            },
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

        camera_buffer.Upload(&ubo);

        // TODO : Setup graph and target swapchain.
        auto const image_semaphore   = frame.GetSemaphorePool().Request();
        auto const present_semaphore = frame.GetSemaphorePool().Request();

        if (m_window->AcquireNextImage(image_semaphore))
        {
            m_renderer->GetGraph()->SetFinalTarget(m_window->GetImage());

            m_renderer->GetGraph()->Build();
            m_renderer->GetGraph()->Execute(image_semaphore, present_semaphore);

            m_window->Present(present_semaphore);
        }

        frame.GetSemaphorePool().Release(image_semaphore);
        frame.GetSemaphorePool().Release(present_semaphore);
    }

    m_renderer->GetGraph()->EndFrame();
}