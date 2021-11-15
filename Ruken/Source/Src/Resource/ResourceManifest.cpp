
#include "Resource/ResourceManifest.hpp"

USING_RUKEN_NAMESPACE

ResourceManifest::ResourceManifest() noexcept:
#ifdef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
    m_identifier    {""},
#endif
    data            {nullptr},
    reference_count {0},
    gc_strategy        {EResourceGCStrategy::ReferenceCount},
    status            {EResourceStatus::Invalid}
{}

ResourceManifest::ResourceManifest(ResourceIdentifier const& in_identifier, class IResource* in_data, EResourceGCStrategy const in_gc_strategy) noexcept:
#ifdef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
    m_identifier    {in_identifier},
#endif
    data            {in_data},
    reference_count {0},
    gc_strategy        {in_gc_strategy},
    status            {EResourceStatus::Invalid}
{}

#ifdef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER

ResourceIdentifier const& ResourceManifest::GetIdentifier() const noexcept
{
    return m_identifier;
}

#else

ResourceIdentifier ResourceManifest::GetIdentifier() const noexcept
{
    return ResourceIdentifier("");
}

#endif