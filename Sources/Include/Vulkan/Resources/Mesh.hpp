
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