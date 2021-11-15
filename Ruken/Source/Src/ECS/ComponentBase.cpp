
#include "ECS/ComponentBase.hpp"

USING_RUKEN_NAMESPACE

ComponentBase::ComponentBase(Archetype const* in_owning_archetype) noexcept:
    m_owning_archetype {in_owning_archetype}
{ }
