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

#include <memory>
#include <string>

#include "Resource/IResource.hpp"
#include "Resource/ResourceLoadingDescriptor.hpp"

#include "Vulkan/Core/Image.hpp"

BEGIN_DAEMON_NAMESPACE

class TextureLoadingDescriptor final : public ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::string path;

        #pragma endregion
};

class Texture final : public IResource
{
    private:

        #pragma region Members

        std::unique_ptr<Image> m_image;

        #pragma endregion

        #pragma region Methods

        DAEvoid UploadData(DAEvoid const*   in_pixels,
                           DAEuint32        in_width,
                           DAEuint32        in_height);

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Texture() = default;

        explicit Texture(std::string const& in_filename);

        Texture(Texture const&  in_copy) = delete;
        Texture(Texture&&       in_move) noexcept;

        ~Texture() = default;

        #pragma endregion

        #pragma region Methods

        DAEvoid Load(class ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        DAEvoid Reload(class ResourceManager& in_manager) override;

        DAEvoid Unload(class ResourceManager& in_manager) noexcept override;

        [[nodiscard]]
        Image const& GetImage() const noexcept;

        #pragma endregion

        #pragma region Operators

        Texture& operator=(Texture const&   in_copy) = delete;
        Texture& operator=(Texture&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE