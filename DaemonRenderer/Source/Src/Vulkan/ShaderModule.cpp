/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Vulkan/ShaderModule.hpp"

#include "Resource/ResourceManager.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

ShaderModule::ShaderModule() :
    m_handle        { nullptr },
    m_identifier    { "" }
{

}

#pragma endregion

#pragma region Methods

DAEvoid ShaderModule::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    // TODO : Read string from GLSL shader on disk.

    // TODO : Compile GLSL shader to SPIR-V.
}

DAEvoid ShaderModule::Reload(ResourceManager& in_manager)
{

}

DAEvoid ShaderModule::Unload(ResourceManager& in_manager) noexcept
{

}

#pragma endregion