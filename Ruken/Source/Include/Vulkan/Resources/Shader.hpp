
#pragma once

#include <optional>

#include "Resource/IResource.hpp"

#include "Vulkan/Core/VulkanShaderModule.hpp"

#include "Vulkan/Resources/ShaderLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class Shader final : public IResource
{
    private:

        #pragma region Members

        std::optional<ShaderLoadingDescriptor>  m_loading_descriptor;
        std::optional<VulkanShaderModule>       m_module;

        #pragma endregion

    public:

        #pragma region Constructors

        Shader() = default;

        Shader(Shader const&    in_copy) = delete;
        Shader(Shader&&         in_move) = delete;

        ~Shader() = default;

        #pragma endregion

        #pragma region Methods

        RkVoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        RkVoid Reload(ResourceManager& in_manager) override;

        RkVoid Unload(ResourceManager& in_manager) noexcept override;

        [[nodiscard]]
        VulkanShaderModule const& GetModule() const noexcept;

        #pragma endregion

        #pragma region Operators

        Shader& operator=(Shader const& in_copy) = delete;
        Shader& operator=(Shader&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE