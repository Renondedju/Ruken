#pragma once

#include "Rendering/RenderDefines.hpp"

#include "Resource/IResource.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Shader final : public IResource
{
    private:

        #pragma region Members

        RenderDevice* m_device;

        vk::ShaderModule m_module;

        #pragma endregion

    public:

        #pragma region Constructors

        Shader(RenderDevice* in_device, std::string_view in_path) noexcept;

        Shader(Shader const& in_copy) = default;
        Shader(Shader&&      in_move) = default;

        ~Shader() noexcept override;

        #pragma endregion

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        vk::PipelineShaderStageCreateInfo GetShaderStage() const noexcept;

        #pragma region Operators

        Shader& operator=(Shader const& in_copy) = default;
        Shader& operator=(Shader&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE