#pragma once

#include "Rendering/Resources/DeviceObjectBase.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Texture : public DeviceObjectBase
{
    private:

        #pragma region Members

        vk::Image          m_image;
        vk::ImageView      m_image_view;
        vk::Sampler        m_image_sampler;
        vk::Allocation     m_allocation;
        vk::AllocationInfo m_allocation_info;

        #pragma endregion

    public:

        #pragma region Constructors

        Texture(RenderDevice* in_device, std::string_view in_path) noexcept;

        Texture(Texture const& in_copy) = delete;
        Texture(Texture&&      in_move) = delete;

        ~Texture() noexcept override;

        #pragma endregion

        vk::Image     const& GetImage       () const noexcept;
        vk::ImageView const& GetImageView   () const noexcept;
        vk::Sampler   const& GetImageSampler() const noexcept;

        #pragma region Operators

        Texture& operator=(Texture const& in_copy) = delete;
        Texture& operator=(Texture&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE