#include "Rendering/Resources/Material.hpp"

USING_RUKEN_NAMESPACE

Material::Material(RenderDevice* in_device, std::string_view in_path) noexcept:
    m_device {in_device}
{
    
}

Material::~Material() noexcept
{
    
}

RkVoid Material::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    
}

RkVoid Material::Reload(ResourceManager& in_manager)
{
    
}

RkVoid Material::Unload(ResourceManager& in_manager) noexcept
{
    
}
