/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

template <template <RkSize...> class TPack, RkSize... TIndices, ComponentFieldType... TFields>
ComponentView<TPack<TIndices...>, TFields...>::ComponentView(Archetype const& in_archetype, FieldChunk<TFields>*... in_fields) noexcept:
    m_fields_references   {ReferencePair<TFields>(0ULL, in_fields)...},
    m_next_empty_range    {in_archetype.GetFreeEntitiesRanges().cbegin()},
    m_component_archetype {in_archetype}
{ }

#pragma region Methods

template <template <RkSize...> class TPack, RkSize... TIndices, ComponentFieldType... TFields>
RkBool ComponentView<TPack<TIndices...>, TFields...>::FindNextEntity() noexcept
{
    // Stores the increase required in 
    RkSize increase_to_next {1ULL};

    // If we need to take care of free entities and if we are on the border of a de-allocated entity range
    if (m_next_empty_range != m_component_archetype.GetFreeEntitiesRanges().cend() && m_index == m_next_empty_range->begin)
            increase_to_next = m_next_empty_range->size;

    ([increase_to_next](ReferencePair<TFields>& in_pair)
    {
        // Computing the number of jumps to do
        RkSize const jumps = (in_pair.first + increase_to_next) / FieldChunk<TFields>::element_count;

        for(RkSize index = 0ULL; index < jumps; ++index)
            in_pair.second = in_pair.second->next_node;

        in_pair.first = (in_pair.first + increase_to_next) % FieldChunk<TFields>::element_count;

    }(std::get<ReferencePair<TFields>>(m_fields_references)), ...);

    m_index += increase_to_next;

    return m_index != m_component_archetype.GetEntitiesCount();
}

template <template <RkSize...> class TPack, RkSize... TIndices, ComponentFieldType... TFields>
template <ComponentFieldType TField>
typename ComponentView<TPack<TIndices...>, TFields...>::template FieldAccess<TField>& ComponentView<TPack<TIndices...>, TFields...>::Fetch() const noexcept
{
    ReferencePair<TField> pair = std::get<ReferencePair<TField>>(m_fields_references);

    return pair.second->data[pair.first];
}

#pragma endregion 