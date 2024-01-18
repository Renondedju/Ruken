template <ComponentFieldType... TFields>
template <RkSize... TIds>
RkSize Component<TFields...>::EnsureStorageSpaceHelper(RkSize in_size, std::index_sequence<TIds...>) noexcept
{
    return static_cast<RkSize>(MinExceptZero(
        { // Initializer list containing all the lambda invocations
            [in_size]<typename TField>(FieldContainerType<TField>& in_list)
            {
                // Computes the chunk id required to store all the requested data
                RkSize const chunk_id      { in_size / in_list.chunk_element_count };
                RkSize const chunk_missing { chunk_id + 1 - in_list.GetSize() };

                // If the size isn't big enough, allocating some more nodes to fit everything
                for (RkSize index = 0; index < chunk_missing; ++index)
                    in_list.CreateNode();

                // Returning the amount of elements created
                return static_cast<RkFloat>(chunk_missing * in_list.chunk_element_count);

            }.template operator()<std::tuple_element_t<TIds, std::tuple<TFields...>>>(std::get<TIds>(m_storage))...
        }
    ));
}

template <ComponentFieldType... TFields>
Component<TFields...>::Component(Archetype const* in_owning_archetype) noexcept:
    ComponentBase {in_owning_archetype}
{ }  

template <ComponentFieldType... TFields>
RkSize Component<TFields...>::EnsureStorageSpace(RkSize const in_size) noexcept
{
    return EnsureStorageSpaceHelper(in_size, std::make_index_sequence<sizeof...(TFields)>());
}

template <ComponentFieldType... TFields>
template <ComponentFieldType TField>
typename Component<TFields...>::template FieldContainerType<TField>& Component<TFields...>::GetFieldContainer() noexcept
{
    return std::get<Helper::template FieldIndex<TField>::value>(m_storage);
}
