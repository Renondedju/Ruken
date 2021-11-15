
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkImage' object.
 *        Images represent multidimensional - up to 3 - arrays of data which can be used for various purposes(e.g. attachments, textures),
 *        by binding them to a graphics or compute pipeline via descriptor sets,
 *        or by directly specifying them as parameters to certain commands.
 * \note  An image must be allocated from the DeviceMemoryAllocator and the memory will be freed at object's destruction.
 */
class VulkanImage
{
    private:

        #pragma region Members

        VkImage       m_handle     {nullptr};
        VmaAllocator  m_allocator  {nullptr};
        VmaAllocation m_allocation {nullptr};
        VkImageType   m_type       {};
        VkFormat      m_format     {VK_FORMAT_UNDEFINED};
        VkExtent3D    m_extent     {};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanImage(VkImage    in_handle,
                             VkFormat   in_format,
                             VkExtent3D in_extent) noexcept;

        explicit VulkanImage(VkImage       in_handle,
                             VmaAllocator  in_allocator,
                             VmaAllocation in_allocation,
                             VkFormat      in_format,
                             VkExtent3D    in_extent) noexcept;

        VulkanImage(VulkanImage const& in_copy) = delete;
        VulkanImage(VulkanImage&&      in_move) noexcept;

        ~VulkanImage() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] VkImage     const& GetHandle() const noexcept;
        [[nodiscard]] VkImageType const& GetType  () const noexcept;
        [[nodiscard]] VkFormat    const& GetFormat() const noexcept;
        [[nodiscard]] VkExtent3D  const& GetExtent() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanImage& operator=(VulkanImage const& in_copy) = delete;
        VulkanImage& operator=(VulkanImage&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE