
template <SystemType TSystem>
RkVoid EntityAdmin::CreateSystem() noexcept
{
    std::unique_ptr<TSystem> system = std::make_unique<TSystem>(*this);

    m_systems.emplace_back(std::move(system));

    // Binding relevant archetypes
    for (auto& archetype_ptr: m_archetypes | std::views::values)
        system->BindArchetype(*archetype_ptr);
}

template <AnyComponentType... TComponents>
Archetype* EntityAdmin::CreateArchetype() noexcept
{
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    // Creating the actual instance
    std::unique_ptr<Archetype> new_archetype = std::make_unique<Archetype>(Tag<TComponents...>());

    // We need to get the pointer before moving it
    Archetype* archetype_ptr = new_archetype.get();
    m_archetypes[targeted_fingerprint] = std::move(new_archetype);

    // Binding the archetype to every relevant system
    for (auto&& system: m_systems)
        system->BindArchetype(*archetype_ptr);

    return archetype_ptr; 
}

template <AnyComponentType... TComponents>
Entity EntityAdmin::CreateEntity() noexcept
{
    // Looking for the archetype of the entity
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    Archetype* target_archetype;

    // If we didn't found any corresponding archetypes, creating it
    if (m_archetypes.find(targeted_fingerprint) == m_archetypes.end())
        target_archetype = CreateArchetype<TComponents...>();
    else
        target_archetype = m_archetypes[targeted_fingerprint].get();

    return target_archetype->CreateEntity();
}

template <ExclusiveComponentType TComponent>
TComponent& EntityAdmin::GetExclusiveComponent() noexcept
{
    // If we didn't found any corresponding component, creating it
    if (m_exclusive_components.find(TComponent::GetId()) == m_exclusive_components.end())
        m_exclusive_components.emplace(TComponent::GetId(), std::make_unique<TComponent>());

    return *static_cast<TComponent*>(m_exclusive_components.at(TComponent::GetId()).get());
}
