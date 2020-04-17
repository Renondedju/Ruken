/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <fstream>

#include <glslang/Public/ShaderLang.h>

#include "Vulkan/Resources/Shader.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

#include "Resource/ResourceProcessingFailure.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

#pragma warning (disable : 4100)

DAEvoid Shader::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    auto const& descriptor = reinterpret_cast<ShaderLoadingDescriptor const&>(in_descriptor);

    std::ifstream stream(descriptor.filename);

    if (!glslang::InitializeProcess())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");



    glslang::FinalizeProcess();

    VkShaderModuleCreateInfo shader_module_create_info = {};

    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = m_code.size() * sizeof(DAEuint32);
    shader_module_create_info.pCode    = m_code.data();

    if (!VK_CHECK(vkCreateShaderModule(VulkanLoader::GetLoadedDevice(), &shader_module_create_info, nullptr, &m_handle)))
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other, false, "");
}

DAEvoid Shader::Reload(ResourceManager& in_manager)
{

}

DAEvoid Shader::Unload(ResourceManager& in_manager) noexcept
{
    m_code.clear();

    if (m_handle)
        vkDestroyShaderModule(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma warning (default : 4100)

VkShaderModule const& Shader::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion