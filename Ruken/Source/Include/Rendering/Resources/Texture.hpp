#pragma once

#include "Rendering/RenderDefines.hpp"

#include "Resource/IResource.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Texture final : public IResource
{
    private:

        #pragma region Members

        RenderDevice* m_device;

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

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        vk::Image     const& GetImage       () const noexcept;
        vk::ImageView const& GetImageView   () const noexcept;
        vk::Sampler   const& GetImageSampler() const noexcept;

        #pragma region Operators

        Texture& operator=(Texture const& in_copy) = delete;
        Texture& operator=(Texture&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE