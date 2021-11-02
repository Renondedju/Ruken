#pragma once

#include "Rendering/RenderObjects/Image.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class RenderTarget
{
    private:

        RenderDevice* m_device;

        vk::Extent3D   m_extent;
        std::unique_ptr<Image>         m_image;
        vk::ImageView  m_image_view;

    public:

        #pragma region Constructors

        RenderTarget(RenderDevice* in_device, RkUint32 in_width, RkUint32 in_height, vk::Format in_format, vk::ImageUsageFlags in_usage) noexcept;

        RenderTarget(RenderTarget const& in_copy) = delete;
        RenderTarget(RenderTarget&&      in_move) = delete;

        ~RenderTarget() noexcept;

        #pragma endregion

        vk::Extent3D  const& GetExtent     () const noexcept;
        vk::Image     const& GetImage      () const noexcept;
        vk::ImageView const& GetImageView  () const noexcept;

        #pragma region Operators

        RenderTarget& operator=(RenderTarget const& in_copy) = delete;
        RenderTarget& operator=(RenderTarget&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE