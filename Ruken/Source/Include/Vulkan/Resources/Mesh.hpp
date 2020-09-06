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

#include <vector>
#include <optional>

#include "Resource/IResource.hpp"

#include "Vulkan/Core/VulkanBuffer.hpp"

#include "Vulkan/Resources/Vertex.hpp"
#include "Vulkan/Resources/MeshLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanDeviceAllocator;

class Mesh final : public IResource
{
    private:

        #pragma region Members

        std::optional<MeshLoadingDescriptor>    m_loading_descriptor;
        std::optional<VulkanBuffer>             m_vertex_buffer;
        std::optional<VulkanBuffer>             m_index_buffer;

        #pragma endregion

        #pragma region Methods

        static std::optional<VulkanBuffer> CreateStagingBuffer  (VulkanDeviceAllocator const& in_allocator, RkUint64 in_size) noexcept;
        static std::optional<VulkanBuffer> CreateVertexBuffer   (VulkanDeviceAllocator const& in_allocator, RkUint64 in_size) noexcept;
        static std::optional<VulkanBuffer> CreateIndexBuffer    (VulkanDeviceAllocator const& in_allocator, RkUint64 in_size) noexcept;

        RkVoid UploadData(VulkanDevice           const& in_device,
                           VulkanDeviceAllocator  const& in_allocator,
                           std::vector<Vertex>    const& in_vertices,
                           std::vector<RkUint32> const& in_indices) const;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Mesh() = default;

        Mesh(Mesh const&    in_copy) = delete;
        Mesh(Mesh&&         in_move) = delete;

        ~Mesh() = default;

        #pragma endregion

        #pragma region Methods

        RkVoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        RkVoid Reload(ResourceManager& in_manager) override;

        RkVoid Unload(ResourceManager& in_manager) noexcept override;

        [[nodiscard]]
        VulkanBuffer const& GetVertexBuffer() const noexcept;

        [[nodiscard]]
        VulkanBuffer const& GetIndexBuffer() const noexcept;

        #pragma endregion

        #pragma region Operators

        Mesh& operator=(Mesh const& in_copy) = delete;
        Mesh& operator=(Mesh&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE