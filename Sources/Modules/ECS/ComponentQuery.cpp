
#include "ECS/ComponentQuery.hpp"
#include "ECS/Archetype.hpp"

USING_RUKEN_NAMESPACE

RkBool ComponentQuery::Match(Archetype const& in_archetype) const noexcept
{
    // Checking inclusion
    if (!in_archetype.GetFingerprint().HasAll(m_included))
        return false;

    // Checking exclusion
    if (in_archetype.GetFingerprint().HasOne(m_excluded))
        return false;

    return true;
}
