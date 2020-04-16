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

#pragma once

#include <string>
#include <vector>

#include "Resource/Handle.hpp"

#include "Vulkan/Resources/Mesh.hpp"
#include "Vulkan/Resources/Material.hpp"

BEGIN_DAEMON_NAMESPACE

class ModelLoadingDescriptor final : public ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::string path;

        #pragma endregion
};

class Model final : public IResource
{
    private:

        #pragma region Members

        std::vector<Handle<Mesh>>       m_meshes;
        std::vector<Handle<Material>>   m_materials;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Model() = default;

        Model(Model const&  in_copy) = delete;
        Model(Model&&       in_move) = delete;

        ~Model() = default;

        #pragma endregion

        #pragma region Methods

        DAEvoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        DAEvoid Reload(ResourceManager& in_manager) override;

        DAEvoid Unload(ResourceManager& in_manager) noexcept override;

        #pragma endregion

        #pragma region Operators

        Model& operator=(Model const&   in_copy) = delete;
        Model& operator=(Model&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE