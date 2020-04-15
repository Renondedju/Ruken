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

#include <glslang/Public/ShaderLang.h>

#include "Vulkan/Resources/Shader.hpp"

#include "Vulkan/Utilities/Debug.hpp"

#include "Resource/ResourceManager.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

DAEvoid Shader::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;

    auto const& descriptor = reinterpret_cast<ShaderLoadingDescriptor const&>(in_descriptor);

    m_filename = descriptor.filename;

    VkShaderModuleCreateInfo shader_module_create_info = {};

    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = 0ull;
    shader_module_create_info.pCode    = nullptr;

    VK_CHECK(vkCreateShaderModule(Loader::GetLoadedDevice(), &shader_module_create_info, nullptr, &m_handle));
}

DAEvoid Shader::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

DAEvoid Shader::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;

    if (!m_handle)
        return;

    vkDestroyShaderModule(Loader::GetLoadedDevice(), m_handle, nullptr);
}

VkShaderModule const& Shader::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion