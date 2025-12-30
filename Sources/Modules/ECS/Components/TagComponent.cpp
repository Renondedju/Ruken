#include "TagComponent.hpp"

USING_RUKEN_NAMESPACE

TagComponent::TagComponent(Archetype const* in_owning_archetype) noexcept:
    Component {in_owning_archetype}
{ }

RkSize TagComponent::EnsureStorageSpace(RkSize) noexcept
{
    return 0ULL;
}
