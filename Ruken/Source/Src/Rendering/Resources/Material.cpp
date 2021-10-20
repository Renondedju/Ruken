#include "Rendering/Resources/Material.hpp"

#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

Material::Material(RenderDevice* in_device, std::string_view const in_path) noexcept:
    m_device {in_device}
{
    (void)in_path;
}

Material::~Material() noexcept
{

}

RkVoid Material::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid Material::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid Material::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}