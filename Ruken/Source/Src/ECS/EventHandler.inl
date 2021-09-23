/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

template <EEventName TEventName, ComponentFieldType... TFields>
EventHandler<TEventName, TFields...>::EventHandler() noexcept
{
    // Component types that should be included in the component query (Archetype query) 
    using QueryComponents = decltype(std::tuple_cat(std::declval<Components>(), std::declval<TagComponents>()));

    // This lambda just helps unwrapping the IterativeComponents tuple into the SetupInclusionQuery function call
    [&]<RkSize... TIds>(std::index_sequence<TIds...>){
        m_query.SetupInclusionQuery<std::tuple_element_t<TIds, QueryComponents>...>();
    }(std::make_index_sequence<std::tuple_size_v<QueryComponents>>());
}

template <EEventName TEventName, ComponentFieldType... TFields>
template<typename TFunction, ComponentFieldType... TInvokedFields>
RkVoid EventHandler<TEventName, TFields...>::Foreach(TFunction&& in_lambda, Tag<TInvokedFields...>) noexcept
{
    for (auto&& archetype: m_archetypes)
        for (ComponentView<TInvokedFields...> view(archetype); !view.IterationDone(); view.FindNextEntity())
            in_lambda(view.template Fetch<TInvokedFields>()...);
}

/*
template <EEventName TEventName, ComponentFieldType... TFields>
RkBool EventHandler<TEventName, TFields...>::FindNextEntity() noexcept
{
    // Checks if the next entity exists
    if (m_entity_index + 1 >= m_archetype_iterator->get().GetEntitiesCount())
    {
        // Checking if we can jump to another archetype
        if (++m_archetype_iterator != m_archetypes.cend())
        {
            SelectArchetype(m_archetype_iterator->get());

            return true;
        }

        // Otherwise there is nothing left to iterate on
        return false;
    }

    // Otherwise, we just continue to iterate in the current archetype
    RkSize increase_to_next {1ULL};

    // If we need to take care of free entities and if we are on the border of a de-allocated entity range
    if (m_next_empty_range != m_archetype_iterator->get().GetEntitiesRanges().cend() && m_entity_index == m_next_empty_range->begin)
            increase_to_next = m_next_empty_range->size;

    ([increase_to_next](ReferencePair<TFields>& in_pair)
    {
        // Computing the number of jumps to do (from chunk to chunk)
        RkSize const jumps = (in_pair.first + increase_to_next) / FieldChunk<TFields>::element_count;

        for(RkSize index = 0ULL; index < jumps; ++index)
            in_pair.second = in_pair.second->next_node;

        in_pair.first = (in_pair.first + increase_to_next) % FieldChunk<TFields>::element_count;

    }(std::get<ReferencePair<TFields>>(m_fields_references)), ...);

    m_entity_index += increase_to_next;

    return true;
}

template <EEventName TEventName, ComponentFieldType... TFields>
template <ComponentFieldType TField>
typename EventHandler<TEventName, TFields...>::template FieldAccess<TField>& EventHandler<TEventName, TFields...>::Fetch() noexcept
{
    ReferencePair<TField> pair = std::get<ReferencePair<TField>>(m_fields_references);

    return pair.second->data[pair.first];
}

template <EEventName TEventName, ComponentFieldType... TFields>
RkVoid EventHandler<TEventName, TFields...>::SelectArchetype(Archetype& in_archetype) noexcept
{
    m_next_empty_range = in_archetype.GetEntitiesRanges().cbegin();
    m_entity_index     = 0ULL;

    // Iterating over every field reference pair
    ([&](ReferencePair<TFields>& in_pair) {

        in_pair.first  = 0ULL;
        in_pair.second = in_archetype.GetComponent<TFields::Component>().template GetFieldContainer<TFields>().GetHead();

    }(std::get<ReferencePair<TFields>>(m_fields_references)), ...);
}

*/
template <EEventName TEventName, ComponentFieldType... TFields>
RkVoid EventHandler<TEventName, TFields...>::AddArchetypeReference(Archetype& in_archetype) noexcept
{
    m_archetypes.emplace_front(std::ref(in_archetype));
}

template <EEventName TEventName, ComponentFieldType... TFields>
EEventName EventHandler<TEventName, TFields...>::GetHandledEvent() noexcept
{
    return TEventName;
}
