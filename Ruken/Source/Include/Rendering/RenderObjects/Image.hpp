#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Image
{
    protected:

        RenderDevice* m_device {nullptr};

        vk::Image         m_handle          {};
        VmaAllocation     m_allocation      {};
        VmaAllocationInfo m_allocation_info {};

    public:

        #pragma region Constructors

        Image(RenderDevice* in_device, vk::ImageCreateInfo const& in_image_create_info) noexcept;

        Image(Image const& in_copy) = delete;
        Image(Image&&      in_move) = delete;

        virtual ~Image() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::Image const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        Image& operator=(Image const& in_copy) = delete;
        Image& operator=(Image&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE