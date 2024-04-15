
#include "Vulkan/Resources/MeshLoadingDescriptor.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructor

MeshLoadingDescriptor::MeshLoadingDescriptor(Renderer const& in_renderer, RkChar const* in_path) noexcept:
    renderer    {in_renderer},
    path        {in_path}
{
    
}

#pragma endregion