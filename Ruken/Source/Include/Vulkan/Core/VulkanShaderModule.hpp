
#pragma once

#include <vector>

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkShaderModule' object.
 *        Shader modules contain shader code and one or more entry points.
 *        Shaders are selected from a shader module by specifying an entry point as part of pipeline creation.
 *        The stages of a pipeline can use shaders that come from different modules.
 *        The shader code defining a shader module must be in the SPIR-V format.
 */
class VulkanShaderModule
{
    private:

        #pragma region Members

        VkShaderModule m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanShaderModule(std::vector<RkUint32> const& in_code) noexcept;

        VulkanShaderModule(VulkanShaderModule const& in_copy) = delete;
        VulkanShaderModule(VulkanShaderModule&&      in_move) noexcept;

        ~VulkanShaderModule() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] RkBool               IsValid  () const noexcept;
        [[nodiscard]] VkShaderModule const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanShaderModule& operator=(VulkanShaderModule const& in_copy) = delete;
        VulkanShaderModule& operator=(VulkanShaderModule&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE