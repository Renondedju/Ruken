#pragma once

template <IsSystem TSystem>
RkVoid Universe::CreateSystem() noexcept
{
    m_systems.emplace_back(std::make_unique<TSystem>(*this));
}

template <IsComponent... TComponents>
Archetype* Universe::CreateArchetype() noexcept
{
    ComponentFingerprint const targeted_fingerprint = ComponentFingerprint::CreateFingerPrintFrom<TComponents...>();

    // Creating the actual instance
    std::unique_ptr<Archetype> new_archetype {
        new Archetype(Empty<TComponents...>{})
    };

    // We need to get the pointer before moving it
    Archetype* archetype_ptr = new_archetype.get();
    m_archetypes[targeted_fingerprint] = std::move(new_archetype);

    return archetype_ptr;
}

template <IsComponent... TComponents>
RkVoid Universe::CreateEntities(RkSize const in_count) noexcept
{
    // Looking for the archetype of the entity
    ComponentFingerprint const targeted_fingerprint = ComponentFingerprint::CreateFingerPrintFrom<TComponents...>();

    Archetype* target_archetype;

    // If we didn't find any corresponding archetypes, creating it
    if (!m_archetypes.contains(targeted_fingerprint))
        target_archetype = CreateArchetype<TComponents...>();
    else
        target_archetype = m_archetypes[targeted_fingerprint].get();

    target_archetype->CreateEntities(in_count);
}

template<IsUniverseComponent TComponent>
TComponent& Universe::GetUniverseComponent() noexcept
{
    // If we didn't find any corresponding component, creating it
    if (!m_components.contains(ComponentIDFactory::StaticID<TComponent>()))
        m_components.emplace(ComponentIDFactory::StaticID<TComponent>(), std::make_unique<TComponent>());

    return *static_cast<TComponent*>(m_components.at(ComponentIDFactory::StaticID<TComponent>()).get());
}
