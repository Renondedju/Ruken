
template <AnyComponentType... TComponents>
Archetype::Archetype(Tag<TComponents...>) noexcept:
    m_fingerprint {ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>()}
{
    // Setup components
    (m_components.try_emplace(TComponents::GetId(), std::make_unique<TComponents>(this)), ...);
}

template <AnyComponentType... TComponents>
Group<TComponents...> Archetype::CreateGroupReference() noexcept
{
    return Group<TComponents...>(*this, GetComponent<TComponents>()...);
}

template <AnyComponentType TComponent>
TComponent& Archetype::GetComponent() noexcept
{
    return static_cast<TComponent&>(*m_components[TComponent::GetId()]);
}
