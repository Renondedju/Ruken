
template <ComponentFieldType... TMembers>
SparseComponent<TMembers...>::SparseComponent(Archetype const& in_owning_archetype) noexcept:
    ComponentBase {&in_owning_archetype}
{ }  

template <ComponentFieldType... TMembers>
RkSize SparseComponent<TMembers...>::EnsureStorageSpace(RkSize const in_size) noexcept
{
    return Layout::EnsureStorageSpace(m_storage, in_size);
}

template <ComponentFieldType... TMembers>
template <ViewType TView>
TView SparseComponent<TMembers...>::GetView() noexcept
{
    return Layout::template GetView<TView>(m_storage, *m_owning_archetype);
}

template <ComponentFieldType... TMembers>
template <ReadonlyViewType TView>
TView SparseComponent<TMembers...>::GetView() const noexcept
{
    return Layout::template GetView<TView>(m_storage, *m_owning_archetype);
}