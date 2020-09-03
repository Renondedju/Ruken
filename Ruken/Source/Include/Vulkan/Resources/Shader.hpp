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