
#include "Vulkan/Core/VulkanImage.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanImage::VulkanImage(VkImage          in_handle,
                         VkFormat   const in_format,
                         VkExtent3D const in_extent) noexcept:
    m_handle {in_handle},
    m_format {in_format},
    m_extent {in_extent}
{

}

VulkanImage::VulkanImage(VkImage          in_handle,
                         VmaAllocator     in_allocator,
                         VmaAllocation    in_allocation,
                         VkFormat   const in_format,
                         VkExtent3D const in_extent) noexcept:
    m_handle     {in_handle},
    m_allocator  {in_allocator},
    m_allocation {in_allocation},
    m_format     {in_format},
    m_extent     {in_extent}
{

}

VulkanImage::VulkanImage(VulkanImage&& in_move) noexcept:
    m_handle     {in_move.m_handle},
    m_allocator  {in_move.m_allocator},
    m_allocation {in_move.m_allocation},
    m_extent     {in_move.m_extent}
{
    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;
}

VulkanImage::~VulkanImage() noexcept
{
    if (!m_handle || !m_allocator || !m_allocation)
        return;

    vmaDestroyImage(m_allocator, m_handle, m_allocation);
}

#pragma endregion

#pragma region Methods

VkImage const& VulkanImage::GetHandle() const noexcept
{
    return m_handle;
}

VkImageType const& VulkanImage::GetType() const noexcept
{
    return m_type;
}

VkFormat const& VulkanImage::GetFormat() const noexcept
{
    return m_format;
}

VkExtent3D const& VulkanImage::GetExtent() const noexcept
{
    return m_extent;
}

#pragma endregion

#pragma region Operators

VulkanImage& VulkanImage::operator=(VulkanImage&& in_move) noexcept
{
    m_handle     = in_move.m_handle;
    m_allocator  = in_move.m_allocator;
    m_allocation = in_move.m_allocation;
    m_type       = in_move.m_type;
    m_format     = in_move.m_format;
    m_extent     = in_move.m_extent;

    in_move.m_handle     = nullptr;
    in_move.m_allocator  = nullptr;
    in_move.m_allocation = nullptr;

    return *this;
}

#pragma endregion