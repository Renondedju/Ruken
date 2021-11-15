
#include "Vulkan/Resources/TextureLoadingDescriptor.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor

TextureLoadingDescriptor::TextureLoadingDescriptor(Renderer const& in_renderer, RkChar const* in_path) noexcept:
    renderer    {in_renderer},
    path        {in_path}
{
    
}

#pragma endregion