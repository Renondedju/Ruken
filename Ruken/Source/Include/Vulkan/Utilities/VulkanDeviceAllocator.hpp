
#pragma once

#include <optional>

#include "Vulkan/Core/VulkanImage.hpp"
#include "Vulkan/Core/VulkanBuffer.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class wraps memory allocation and resource (buffer and image) creation in Vulkan.
 */
class VulkanDeviceAllocator
{
    private:

        #pragma region Members

        VmaAllocator m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanDeviceAllocator(class VulkanPhysicalDevice const& in_physical_device) noexcept;

        VulkanDeviceAllocator(VulkanDeviceAllocator const& in_copy) = delete;
        VulkanDeviceAllocator(VulkanDeviceAllocator&&      in_move) = delete;

        ~VulkanDeviceAllocator() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        std::optional<VulkanImage> CreateImage(VkImageCreateInfo       const& in_image_create_info,
                                               VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        [[nodiscard]]
        std::optional<VulkanBuffer> CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                                                 VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        [[nodiscard]]
        RkBool IsValid() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanDeviceAllocator& operator=(VulkanDeviceAllocator const& in_copy) = delete;
        VulkanDeviceAllocator& operator=(VulkanDeviceAllocator&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE