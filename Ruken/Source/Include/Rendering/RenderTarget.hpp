
#pragma once

#include <memory>

#include "Vulkan/Core/VulkanImage.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderTarget
{
    private:

        #pragma region Members

        std::unique_ptr<VulkanImage> m_image;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderTarget() = default;

        RenderTarget(RenderTarget const&    in_copy) = delete;
        RenderTarget(RenderTarget&&         in_move) = delete;

        ~RenderTarget() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VulkanImage const& GetImage() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderTarget& operator=(RenderTarget const& in_copy) = delete;
        RenderTarget& operator=(RenderTarget&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE