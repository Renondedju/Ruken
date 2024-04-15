
#include "Resource/ResourceManager.hpp"

#include "Vulkan/Resources/Material.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

#pragma warning (disable : 4100)

RkVoid Material::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    m_loading_descriptor = reinterpret_cast<MaterialLoadingDescriptor const&>(in_descriptor);

    // NotImplementedException
}

RkVoid Material::Reload(ResourceManager& in_manager)
{
    // NotImplementedException
}

RkVoid Material::Unload(ResourceManager& in_manager) noexcept
{
    m_loading_descriptor.reset();
}

#pragma warning (default : 4100)

#pragma endregion