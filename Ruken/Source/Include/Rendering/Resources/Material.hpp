#pragma once

#include "Rendering/Resources/Shader.hpp"
#include "Rendering/Resources/Texture.hpp"

#include "Resource/IResource.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

struct MaterialData
{
    RkUint32 albedo_texture;
    RkUint32 normal_texture;
};

class Material final : public IResource
{
    protected:

        RenderDevice* m_device;

        std::vector<std::unique_ptr<Shader>>  m_shaders;
        std::vector<std::unique_ptr<Texture>> m_textures;

    public:

        #pragma region Operators

        Material(RenderDevice* in_device, std::string_view in_path) noexcept;

        Material(Material const& in_copy) = delete;
        Material(Material&&      in_move) = delete;

        ~Material() noexcept override;

        #pragma endregion

        #pragma region Methods

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