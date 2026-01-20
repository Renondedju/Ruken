#include "ECS/Entity.hpp"
#include "ECS/Archetype.hpp"

USING_RUKEN_NAMESPACE

Entity::Entity(Archetype& in_archetype, RkSize const in_index):
    m_archetype        {in_archetype},
    m_index {in_index}
{ }

RkVoid Entity::Delete() const noexcept
{
    m_archetype.DeleteEntity(m_index);
}

Archetype& Entity::GetOwner() const noexcept
{
    return m_archetype;
}

RkSize Entity::GetIndex() const noexcept
{
    return m_index;
}

RkBool Entity::operator==(Entity const& in_other) const noexcept
{
    return &in_other.m_archetype == &m_archetype && in_other.m_index == m_index;
}
