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
    Range& free_range = m_free_ranges.front();

    // Reducing the range, and if it is empty, removing it
    RkSize const location = free_range.begin;
    if (free_range.ReduceRight() == 0ULL)
        m_free_ranges.erase(m_free_ranges.cbegin());

    --m_free_space_count;

    return location;
}

ArchetypeFingerprint const& Archetype::GetFingerprint() const noexcept
{
    return m_fingerprint;
}

Entity Archetype::CreateEntity() noexcept
{
    // If there are no free space in the archetype
    if (m_free_space_count == 0ULL)
    {
        // Allocate a new entity
        for (auto& [id, component]: m_components)
            component->CreateItemAt(m_allocation_size);

        return Entity(*this, m_allocation_size++);
    }

    return Entity(*this, GetFreeEntityLocation());
}

RkSize Archetype::EntitiesCount() const noexcept
{
    return m_allocation_size;
}

#pragma endregion