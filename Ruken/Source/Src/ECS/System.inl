
template <ComponentType... TComponents>
System<TComponents...>::System(EntityAdmin& in_admin) noexcept:
    m_groups {},
    m_admin  {in_admin}
{
    // This lambda just helps unwrapping the IterativeComponents tuple into the SetupInclusionQuery function call
    [&]<RkSize... TIds>(std::index_sequence<TIds...>){
        m_query.SetupInclusionQuery<std::tuple_element_t<TIds, IterativeComponents>...>();
    }(std::make_index_sequence<std::tuple_size_v<IterativeComponents>>());
}

template <ComponentType... TComponents>
RkVoid System<TComponents...>::AddReferenceGroup(Archetype& in_archetype) noexcept
{
    // This lambda just helps unwrapping the IterativeComponents tuple into the CreateGroupReference function call
    [&]<RkSize... TIds>(std::index_sequence<TIds...>){
        m_groups.emplace_back(in_archetype.CreateGroupReference<std::tuple_element_t<TIds, IterativeComponents>...>());
    }(std::make_index_sequence<std::tuple_size_v<IterativeComponents>>());
}

template <ComponentType... TComponents>
template <ExclusiveComponentType TExclusiveComponent>
typename System<TComponents...>::template ExclusiveComponentAccess<TExclusiveComponent>& System<TComponents...>::GetExclusiveComponent() noexcept
{
    return m_admin.GetExclusiveComponent<TExclusiveComponent>();
}