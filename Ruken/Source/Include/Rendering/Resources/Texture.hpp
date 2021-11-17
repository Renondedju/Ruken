#pragma once

#include "Resource/IResource.hpp"

#include "Rendering/RenderObjects/Image.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;

class Texture final : public Image, public IResource
{
    private:

        #pragma region Members

        Renderer* m_renderer;

        vk::Sampler m_sampler;

        #pragma endregion

    public:

        #pragma region Constructors

        Texture(Renderer* in_renderer, std::string_view in_path) noexcept;

        Texture(Texture const& in_copy) = delete;
        Texture(Texture&&      in_move) = delete;

        ~Texture() noexcept override;

        #pragma endregion

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        vk::Sampler const& GetSampler() const noexcept;

        #pragma region Operators

        Texture& operator=(Texture const& in_copy) = delete;
        Texture& operator=(Texture&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE