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
#include <optional>

#include "Vector/Vector.hpp"

#include "Resource/IResource.hpp"
#include "Resource/ResourceLoadingDescriptor.hpp"

#include "Vulkan/Core/VulkanBuffer.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanCommandBuffer;

struct Vertex
{
    Vector3f position;
    Vector3f normal;
    Vector2f uv;
};

class MeshLoadingDescriptor final : public ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::string path;

        #pragma endregion
};

class Mesh final : public IResource
{
    private:

        #pragma region Members

        MeshLoadingDescriptor m_loading_descriptor;

        std::unique_ptr<VulkanBuffer> m_vertex_buffer;
        std::unique_ptr<VulkanBuffer> m_index_buffer;

        #pragma endregion

        #pragma region Methods

        static std::optional<VulkanBuffer> CreateStagingBuffer  (DAEuint64 in_size) noexcept;
        static std::optional<VulkanBuffer> CreateVertexBuffer   (DAEuint64 in_size) noexcept;
        static std::optional<VulkanBuffer> CreateIndexBuffer    (DAEuint64 in_size) noexcept;

        DAEvoid UploadData(std::vector<Vertex>    const& in_vertices,
                           std::vector<DAEuint32> const& in_indices) const;

        DAEvoid CopyBuffers(VulkanCommandBuffer const& in_command_buffer, VulkanBuffer const& in_vertex_buffer, VulkanBuffer const& in_index_buffer) const noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Mesh() = default;

        Mesh(Mesh const&    in_copy) = delete;
        Mesh(Mesh&&         in_move) = delete;

        ~Mesh() = default;

        #pragma endregion

        #pragma region Methods

        DAEvoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        DAEvoid Reload(ResourceManager& in_manager) override;

        DAEvoid Unload(ResourceManager& in_manager) noexcept override;

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

END_DAEMON_NAMESPACE