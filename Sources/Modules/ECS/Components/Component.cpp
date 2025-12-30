#include "Component.hpp"

USING_RUKEN_NAMESPACE

Component::Component(Archetype const* in_owner) noexcept:
    owner {in_owner}
{}
