#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

struct AttachmentInfo
{
    vk::Extent3D extent;
    vk::Format format;
    vk::ImageUsageFlags usage;
};

class Image
{
    protected:

        RenderDevice* m_device {nullptr};

        vk::Extent3D  m_extent {};
        vk::Image     m_handle {};
        vk::ImageView m_view   {};

        VmaAllocation     m_allocation      {};
        VmaAllocationInfo m_allocation_info {};

        RkVoid CreateImage(vk::ImageCreateInfo     const& in_image_create_info) noexcept;
        RkVoid CreateView (vk::ImageViewCreateInfo const& in_view_create_info) noexcept;

    public:

        #pragma region Constructors

        Image(RenderDevice* in_device) noexcept;
        Image(RenderDevice* in_device, vk::Extent3D const& in_extent, vk::Image const& in_handle, vk::ImageView const& in_view) noexcept;
        Image(RenderDevice* in_device, vk::ImageCreateInfo const& in_image_create_info) noexcept;

        Image(Image const& in_copy) = delete;
        Image(Image&&      in_move) = delete;

        virtual ~Image() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::Extent3D  const& GetExtent() const noexcept;
        vk::Image     const& GetHandle() const noexcept;
        vk::ImageView const& GetView  () const noexcept;

        #pragma endregion

        #pragma region Operators

        Image& operator=(Image const& in_copy) = delete;
        Image& operator=(Image&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE