﻿/*
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

#include "Resource/ResourceProcessingFailure.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

#pragma warning (disable : 4100)

DAEvoid Shader::Load(ResourceManager& in_manager, Renderer& in_renderer, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<ShaderLoadingDescriptor const&>(in_descriptor);

    if (!glslang::InitializeProcess())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);



    glslang::FinalizeProcess();

    std::vector<DAEuint32> code;

    m_module = std::make_unique<VulkanShaderModule>(code);

    if (!m_module)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);
}

DAEvoid Shader::Reload(ResourceManager& in_manager, Renderer& in_renderer)
{
    if (!glslang::InitializeProcess())
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);



    glslang::FinalizeProcess();

    std::vector<DAEuint32> code;

    m_module = std::make_unique<VulkanShaderModule>(code);

    if (!m_module)
        throw ResourceProcessingFailure(EResourceProcessingFailureCode::Other);

    VulkanDebug::SetObjectName(VK_OBJECT_TYPE_SHADER_MODULE, reinterpret_cast<DAEuint64>(m_module->GetHandle()), "");
}

DAEvoid Shader::Unload(ResourceManager& in_manager, Renderer& in_renderer) noexcept
{
    m_module.reset();
}

#pragma warning (default : 4100)

VulkanShaderModule const& Shader::GetModule() const noexcept
{
    return *m_module;
}

#pragma endregion