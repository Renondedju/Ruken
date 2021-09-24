#pragma once

#include "Rendering/Resources/DeviceObjectBase.hpp"

BEGIN_RUKEN_NAMESPACE

class Shader : public DeviceObjectBase
{
    private:

        #pragma region Members

        vk::ShaderModule m_vertex_module;
        vk::ShaderModule m_fragment_module;

        #pragma endregion

    public:

        #pragma region Constructors

        Shader(RenderDevice* in_device, std::string_view in_path) noexcept;

        Shader(Shader const& in_copy) = default;
        Shader(Shader&&      in_move) = default;

        ~Shader() noexcept override;

        #pragma endregion

        std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages() const;

        #pragma region Operators

        Shader& operator=(Shader const& in_copy) = default;
        Shader& operator=(Shader&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE