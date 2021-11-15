
#pragma once

#include <optional>

#include "Resource/IResource.hpp"

#include "Vulkan/Resources/MaterialLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class Material final : public IResource
{
    private:

        #pragma region Members

        std::optional<MaterialLoadingDescriptor> m_loading_descriptor;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Material() = default;

        Material(Material const&    in_copy) = delete;
        Material(Material&&         in_move) = delete;

        ~Material() = default;

        #pragma endregion

        #pragma region Methods

        RkVoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;

        RkVoid Reload(ResourceManager& in_manager) override;

        RkVoid Unload(ResourceManager& in_manager) noexcept override;

        #pragma endregion

        #pragma region Operators

        Material& operator=(Material const& in_copy) = delete;
        Material& operator=(Material&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE