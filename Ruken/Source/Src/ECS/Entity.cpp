
#include "ECS/Entity.hpp"
#include "ECS/Archetype.hpp"

USING_RUKEN_NAMESPACE

Entity::Entity(Archetype& in_archetype, RkSize const in_local_identifier):
    m_archetype        {in_archetype},
    m_local_identifier {in_local_identifier}
{ }

RkVoid Entity::Delete() const noexcept
{
    m_archetype.DeleteEntity(m_local_identifier);
}

Archetype& Entity::GetOwner() const noexcept
{
    return m_archetype;
}

RkSize Entity::GetLocalIdentifier() const noexcept
{
    return m_local_identifier;
}

RkBool Entity::operator==(Entity const& in_other) const noexcept
{
    return &in_other.m_archetype == &m_archetype && in_other.m_local_identifier == m_local_identifier;
}
