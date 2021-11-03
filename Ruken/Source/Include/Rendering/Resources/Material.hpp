#pragma once

#include "Rendering/Resources/Shader.hpp"
#include "Rendering/Resources/Texture.hpp"

#include "Resource/IResource.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;
class RenderFrame;

struct MaterialData
{
    RkUint32 albedo_texture;
    RkUint32 normal_texture;
};

class Material final : public IResource
{
    protected:

        Renderer* m_renderer;

        vk::DescriptorSetLayout m_descriptor_set_layout;
        vk::PipelineLayout      m_pipeline_layout;
        vk::Pipeline            m_pipeline;

        std::vector<std::unique_ptr<Shader>>  m_shaders;
        std::vector<std::unique_ptr<Texture>> m_textures;

    public:

        #pragma region Operators

        Material(Renderer* in_renderer, std::string_view in_path) noexcept;

        Material(Material const& in_copy) = delete;
        Material(Material&&      in_move) = delete;

        ~Material() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Bind(vk::CommandBuffer const& in_command_buffer) const noexcept;

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        #pragma endregion

        #pragma region Operators

        Material& operator=(Material const& in_copy) = delete;
        Material& operator=(Material&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE