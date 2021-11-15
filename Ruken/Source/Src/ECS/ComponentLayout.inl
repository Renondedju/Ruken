
#pragma region Methods

template <ComponentFieldType... TFields>
template <ViewType TView, RkSize... TIds>
TView ComponentLayout<TFields...>::GetViewHelper(ContainerType const& in_container, Archetype const& in_owning_archetype, std::index_sequence<TIds...>) noexcept
{
    // Guaranteed copy elision
    return TView { in_owning_archetype, std::get<TIds>(in_container).GetHead()... };
}

template <ComponentFieldType... TFields>
template <RkSize... TIds>
RkSize ComponentLayout<TFields...>::EnsureStorageSpaceHelper(ContainerType& in_container, RkSize in_size, std::index_sequence<TIds...>) noexcept
{
    return MinExceptZero<RkSize>(
        { // Initializer list containing all the lambda invocations
            [in_size](LinkedChunkList<typename TFields::Type>& in_list)
            {
                // Computes the chunk id required to store all the requested data
                RkSize const chunk_id      = in_size / in_list.chunk_element_count;
                RkSize const chunk_missing = (chunk_id + 1) - in_list.GetSize();

                // If the size isn't big enough, allocating some more nodes to fit everything
                for (RkSize index = 0; index < chunk_missing; ++index)
                    in_list.CreateNode();

                // Returning the amount of elements created
                return chunk_missing * in_list.chunk_element_count;

            }(std::get<TIds>(in_container))...
        }
    );
}

template <ComponentFieldType... TFields>
template <ViewType TView>
TView ComponentLayout<TFields...>::GetView(ContainerType const& in_container, Archetype const& in_owning_archetype) noexcept
{
    return GetViewHelper<TView>(in_container, in_owning_archetype, typename TView::FieldIndexSequence());
}

template <ComponentFieldType... TFields>
RkSize ComponentLayout<TFields...>::EnsureStorageSpace(ContainerType& in_container, RkSize in_size) noexcept
{
    return EnsureStorageSpaceHelper(in_container, in_size, std::make_index_sequence<sizeof...(TFields)>());
}

#pragma endregion
