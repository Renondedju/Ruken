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

Entity Archetype::CreateEntity() noexcept
{
    if (m_free_space_count == 0ULL)
    {
        // If there are no more free space for entities, that means that we might have to allocate some more
        // to store the new data
        for (auto& [id, component]: m_components)
            component->EnsureStorageSpace(m_entities_count);

        // Creating the entity
        return Entity(*this, m_entities_count++);
    }

    // Otherwise, fetching an entity id from a free location
    return Entity(*this, GetFreeEntityLocation());
}

RkVoid Archetype::DeleteEntity(Entity const& in_entity) noexcept
{
    // Checking if the entity owner (aka archetype) is the right one
    if (&in_entity.GetOwner() != this)
        return;

    ++m_free_space_count;

    // If there are no ranges yet, creating the first one
    if (m_free_entities.empty())
    {
        m_free_entities.emplace_front(in_entity.GetLocalIdentifier(), 1ULL);
        return;
    }

    // Checking if any empty range can be expanded
    for (std::list<Range>::iterator it = m_free_entities.begin(); it != m_free_entities.end(); ++it)
    {
        if (it->begin - 1ULL == in_entity.GetLocalIdentifier())
        {
            it->ExpandLeft();
            return;
        }

        if (it->begin > in_entity.GetLocalIdentifier())
        {
            m_free_entities.insert(it, Range(in_entity.GetLocalIdentifier(), 1ULL));
            return;
        }

        if (it->begin + it->size == in_entity.GetLocalIdentifier())
        {
            it->ExpandRight();
            return;
        }
    }
}

#pragma endregion
