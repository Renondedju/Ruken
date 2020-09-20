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

#include <Common/Numerics.hpp>

#include "ECS/Range.hpp"
#include "ECS/Archetype.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

RkSize Archetype::GetFreeEntityLocation() noexcept
{
    // Checking for a free space
    Range& free_range = m_free_entities.front();

    // Reducing the range, and if it is empty, removing it
    RkSize const location = free_range.begin;
    if (free_range.ReduceRight() == 0ULL)
        m_free_entities.erase(m_free_entities.cbegin());

    --m_free_space_count;

    return location;
}

ArchetypeFingerprint const& Archetype::GetFingerprint() const noexcept
{
    return m_fingerprint;
}

RkSize Archetype::GetEntitiesCount() const noexcept
{
    return m_entities_count;
}

Entity Archetype::CreateEntity() noexcept
{
    // If there are no more free space for entities, that means that we might have to allocate some more
    // to store the new data
    if (m_free_space_count == 0ULL)
    {
        // allocated_elements returns the minimum number of elements a component successfully allocated
        // This is used to reduce the number of calls to this function to the bare minimum
        // speeding up the creation process of an entity
        RkSize allocated_elements = UINT64_MAX;
        for (auto& [id, component]: m_components)
            allocated_elements = MinExceptZero(component->EnsureStorageSpace(m_entities_count + 1), allocated_elements);

        // Since the EnsureStorageSpace is always called when there is no more free entity available, allocated_elements should always
        // be at a minimum of 1, which means that the free space count cannot decrease here
        m_free_space_count += allocated_elements - 1;

        // If the number of allocated elements (entities) is greater than one
        // ie. that some entities are left unconsumed, we need to declare a free entity range for that
        if (allocated_elements > 1)
            m_free_entities.emplace_back(Range(m_entities_count + 1, allocated_elements - 1));

        // Creating the entity
        return Entity(*this, m_entities_count++);
    }

    // Otherwise, fetching an entity id from a free location
    ++m_entities_count;
    return Entity(*this, GetFreeEntityLocation());
}

RkVoid Archetype::DeleteEntity(RkSize in_local_identifier) noexcept
{
    ++m_free_space_count;
    --m_entities_count;

    // If there are no ranges yet, creating the first one
    if (m_free_entities.empty())
    {
        m_free_entities.emplace_front(in_local_identifier, 1ULL);
        return;
    }

    // Checking if any empty range can be expanded
    for (std::list<Range>::iterator it = m_free_entities.begin(); it != m_free_entities.end(); ++it)
    {
        if (it->begin - 1ULL == in_local_identifier)
        {
            it->ExpandLeft();
            return;
        }

        if (it->begin > in_local_identifier)
        {
            m_free_entities.insert(it, Range(in_local_identifier, 1ULL));
            return;
        }

        if (it->begin + it->size == in_local_identifier)
        {
            it->ExpandRight();
            return;
        }
    }
}

std::list<Range> const& Archetype::GetFreeEntitiesRanges() const noexcept
{
    return m_free_entities;
}

#pragma endregion
