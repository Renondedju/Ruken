#pragma once

#include "Rendering/Resources/Material.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;
class RenderFrame;

class MaterialInstance final : public IResource
{
    protected:

        Renderer* m_renderer;
        Material* m_material;

        vk::Pipeline m_pipeline;

    public:

        #pragma region Operators

        MaterialInstance(Renderer* in_renderer, Material* in_material) noexcept;

        MaterialInstance(MaterialInstance const& in_copy) = delete;
        MaterialInstance(MaterialInstance&&      in_move) = delete;

        ~MaterialInstance() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Bind(vk::CommandBuffer const& in_command_buffer) const noexcept;

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        #pragma endregion

        #pragma region Operators

        MaterialInstance& operator=(MaterialInstance const& in_copy) = delete;
        MaterialInstance& operator=(MaterialInstance&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE