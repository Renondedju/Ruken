
#include <ranges>
#include <Common/Numerics.hpp>
#include "Maths/MinMax.hpp"

#include "ECS/Range.hpp"
#include "ECS/Archetype.hpp"

USING_RUKEN_NAMESPACE

#pragma region Methods

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
        for (auto const& component: m_components | std::views::values)
            allocated_elements = MinExceptZero(component->EnsureStorageSpace(m_entities_count + 1), allocated_elements);

        // Since the EnsureStorageSpace is always called when there is no more free entity available, allocated_elements should always
        // be at a minimum of 1, which means that the free space count cannot decrease here
        m_free_space_count += allocated_elements - 1;

        // Signaling that a new entity exists at the end of the archetype containers by increasing the last entity range size by one
        // There is no need for further checks since
        if (m_entities.empty()) // If this is the first allocation, we need to create the first range
            m_entities.emplace_front(Range {0ULL, 0ULL});

        m_entities.back().ExpandRight();

        // Creating the entity
        return Entity(*this, m_entities_count++);
    }

    // Otherwise, fetching an entity id from a free location (located at the end of the first entity range stored)
    auto   const entity_range      {m_entities.begin()};
    RkSize const entity_id         {entity_range->begin + entity_range->ExpandRight() - 1}; // Entity ids are indexes, requiring to be offset by -1
    auto   const next_entity_range {std::next(entity_range)};

    // We need to check if that range can be merged with the next one, if any
    if (next_entity_range != m_entities.cend() && entity_id + 1 == next_entity_range->begin)
    {
        entity_range->size += next_entity_range->size;
        m_entities.erase(next_entity_range);
    }

    // Finally, increasing the number of entities by one and returning the new entity id
    ++m_entities_count;
    --m_free_space_count;

    return Entity(*this, entity_id);
}

RkVoid Archetype::DeleteEntity(RkSize const in_local_identifier) noexcept
{
    ++m_free_space_count;
    --m_entities_count;

    // Locating the first range containing that entity
    decltype(m_entities)::iterator containing_range = m_entities.begin();

    while (!containing_range->Contains(in_local_identifier) && containing_range != m_entities.cend())
        ++containing_range;

    // If no range was found, returning here, there is nothing to delete
    if (containing_range == m_entities.cend())
        return;

    // If the entity is in the middle of a range
    if (in_local_identifier > containing_range->begin && in_local_identifier < containing_range->begin + containing_range->size)
    {
        // Splitting the containing range
        Range const new_range {in_local_identifier + 1, containing_range->size - in_local_identifier - 1 - containing_range->begin};
        containing_range->size -= in_local_identifier - containing_range->begin;

        // Inserting the new range after the one that was containing that entity
        m_entities.insert(++containing_range, new_range);
    }
    // Otherwise if it is right at the start of the range
    else if (in_local_identifier == containing_range->begin)
    {
        if (containing_range->ReduceRight() == 0)
            m_entities.erase(containing_range);
    }
    // Otherwise the entity is located at the end of the range 
    else if (--containing_range->size == 0)
        m_entities.erase(containing_range);
}

std::list<Range> const& Archetype::GetEntitiesRanges() const noexcept
{
    return m_entities;
}

#pragma endregion
