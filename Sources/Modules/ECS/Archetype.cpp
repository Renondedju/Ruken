#include "Core/Maths/MinMax.hpp"

#include "ECS/Range.hpp"
#include "ECS/Archetype.hpp"

#include <ranges>

USING_RUKEN_NAMESPACE

#pragma region Methods

ComponentFingerprint const& Archetype::GetFingerprint() const noexcept
{
    return m_fingerprint;
}

RkSize Archetype::GetChunkSize() const noexcept
{
    return m_entities_per_chunk;
}

RkSize Archetype::GetEntitiesCount() const noexcept
{
    return m_entities_count;
}

RkVoid Archetype::CreateEntities(RkSize const in_count) noexcept
{
    m_entities_count += in_count;

    for (auto const& component_ptr : m_entity_storage | std::views::values)
        component_ptr->CreateEntities(in_count);
}

RkVoid Archetype::DeleteEntity(RkSize const in_index) noexcept
{

}

#pragma endregion
