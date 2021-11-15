
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

        static std::optional<VulkanImage>   CreateImage         (VulkanDeviceAllocator const& in_allocator, RkUint32 in_width, RkUint32 in_height) noexcept;
        static std::optional<VulkanBuffer>  CreateStagingBuffer (VulkanDeviceAllocator const& in_allocator, RkUint64 in_size) noexcept;

        RkVoid UploadData(VulkanDevice          const& in_device,
                           VulkanDeviceAllocator const& in_allocator,
                           RkVoid               const* in_data,
                           RkUint64                    in_size) const;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Texture() = default;

        Texture(Texture const&  in_copy) = delete;
        Texture(Texture&&       in_move) = delete;

        ~Texture() = default;

        #pragma endregion

        #pragma region Methods

        RkVoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        RkVoid Reload(ResourceManager& in_manager) override;

        RkVoid Unload(ResourceManager& in_manager) noexcept override;

        [[nodiscard]]
        VulkanImage const& GetImage() const noexcept;

        #pragma endregion

        #pragma region Operators

        Texture& operator=(Texture const&   in_copy) = delete;
        Texture& operator=(Texture&&        in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE