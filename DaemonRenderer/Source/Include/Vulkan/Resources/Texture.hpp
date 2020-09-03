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

#include <optional>

#include "Resource/IResource.hpp"

#include "Vulkan/Core/VulkanImage.hpp"
#include "Vulkan/Core/VulkanBuffer.hpp"

#include "Vulkan/Resources/TextureLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanDeviceAllocator;

class Texture final : public IResource
{
    private:

        #pragma region Members

        std::optional<TextureLoadingDescriptor> m_loading_descriptor;
        std::optional<VulkanImage>              m_image;

        #pragma endregion

        #pragma region Methods

        static std::optional<VulkanImage>   CreateImage         (VulkanDeviceAllocator const& in_allocator, DAEuint32 in_width, DAEuint32 in_height) noexcept;
        static std::optional<VulkanBuffer>  CreateStagingBuffer (VulkanDeviceAllocator const& in_allocator, DAEuint64 in_size) noexcept;

        DAEvoid UploadData(VulkanDevice          const& in_device,
                           VulkanDeviceAllocator const& in_allocator,
                           DAEvoid               const* in_data,
                           DAEuint64                    in_size) const;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Texture() = default;

        Texture(Texture const&  in_copy) = delete;
        Texture(Texture&&       in_move) = delete;

        ~Texture() = default;

        #pragma endregion

        #pragma region Methods

        DAEvoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        DAEvoid Reload(ResourceManager& in_manager) override;

        DAEvoid Unload(ResourceManager& in_manager) noexcept override;

        [[nodiscard]]
        VulkanImage const& GetImage() const noexcept;

        #pragma endregion

        #pragma region Operators

        Texture& operator=(Texture const&   in_copy) = delete;
        Texture& operator=(Texture&&        in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE