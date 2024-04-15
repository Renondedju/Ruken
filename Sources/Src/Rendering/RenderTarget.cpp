
#include "Rendering/RenderTarget.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor and Destructor

#pragma endregion

#pragma region Methods

VulkanImage const& RenderTarget::GetImage() const noexcept
{
    return *m_image;
}

#pragma endregion