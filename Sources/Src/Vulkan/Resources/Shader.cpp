
#include <fstream>

#include <glslang/Public/ShaderLang.h>

#include "Vulkan/Resources/Shader.hpp"

#include "Resource/ResourceProcessingFailure.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

#pragma warning (disable : 4100)

RkVoid Shader::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<ShaderLoadingDescriptor const&>(in_descriptor);

    // NotImplementedException
}

RkVoid Shader::Reload(ResourceManager& in_manager)
{
    // NotImplementedException
}

RkVoid Shader::Unload(ResourceManager& in_manager) noexcept
{
    m_loading_descriptor.reset();
}

#pragma warning (default : 4100)

VulkanShaderModule const& Shader::GetModule() const noexcept
{
    return *m_module;
}

#pragma endregion