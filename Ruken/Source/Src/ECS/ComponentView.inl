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

#pragma once

template <ComponentFieldType... TFields>
ComponentView<TFields...>::ComponentView(Archetype& in_archetype) noexcept:
    m_fields_references    {ReferencePair<TFields>(0ULL, in_archetype.GetComponent<TFields::Component>().template GetFieldContainer<TFields>().GetHead())...},
    m_current_entity_range {in_archetype.GetEntitiesRanges().cbegin()},
    m_component_archetype  {in_archetype}
{ }

#pragma region Methods

template <ComponentFieldType... TFields>
RkVoid ComponentView<TFields...>::FindNextEntity() noexcept
{
    RkSize jump_size {1ULL};

    // Checking if the next index is outside of the current range
    if (m_index + 1 > m_current_entity_range->begin + m_current_entity_range->size)
    {
        // Jumping to the next range
        ++m_current_entity_range;
        jump_size = m_current_entity_range->begin - m_index;
    }

    m_index += jump_size;

    // Otherwise, jumping to the next entity in range
    ([jump_size]<typename TField>(ReferencePair<TField>& in_pair)
    {
        // Computing the number of jumps to do
        RkSize const jumps = (in_pair.first + jump_size) / FieldChunk<TField>::element_count;

        for(RkSize index = 0ULL; index < jumps; ++index)
            in_pair.second = in_pair.second->next_node;

        in_pair.first = (in_pair.first + jump_size) % FieldChunk<TField>::element_count;

    }.template operator()<ReferencePair<TFields>>(std::get<ReferencePair<TFields>>(m_fields_references)), ...);
}

template <ComponentFieldType ... TFields>
RkBool ComponentView<TFields...>::IterationDone() const noexcept
{
    return m_index == m_component_archetype.GetEntitiesCount();
}

template <ComponentFieldType... TFields>
template <ComponentFieldType TField>
typename ComponentView<TFields...>::template FieldAccess<TField>& ComponentView<TFields...>::Fetch() const noexcept
{
    ReferencePair<TField> pair = std::get<ReferencePair<TField>>(m_fields_references);

    return pair.second->data[pair.first];
}

#pragma endregion 
