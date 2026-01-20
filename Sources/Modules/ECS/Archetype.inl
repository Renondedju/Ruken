#pragma once

#include "ECS/Components/EntityComponent.hpp"
#include "ECS/Components/TagComponent.hpp"
#include "Core/Meta/TupleSubset.hpp"

#include <type_traits>

BEGIN_RUKEN_NAMESPACE

template <IsArchetypeComponent... TComponents>
Archetype::Archetype(Empty<TComponents...>) noexcept:
    m_fingerprint        {ComponentFingerprint::CreateFingerPrintFrom<TComponents...>()},
    m_entities_per_chunk {std::numeric_limits<RkSize>::max()}
{
    using EntityComponents = TupleSubset<EntityComponentType, TComponents...>::Type;

    // Lambda unwraps entity components
    [&]<auto... TValues>(std::index_sequence<TValues...>)
    {
        ([&]<IsEntityComponent TEntityComponent>(Empty<TEntityComponent>)
        {
            // Computing entity chunk sizes
            m_entities_per_chunk = std::min(m_entities_per_chunk, TEntityComponent::min_entities_per_chunk);

            // Setting up storage.
            m_entity_storage.try_emplace(ComponentIDFactory::StaticID<TEntityComponent>(), std::make_unique<TEntityComponent>(m_entities_per_chunk));

        }(Empty<std::tuple_element_t<TValues, EntityComponents>>{}), ...);
    }(std::make_index_sequence<std::tuple_size_v<EntityComponents>>());
}

template <IsEntityComponent TComponent>
TComponent& Archetype::GetComponent() noexcept
{
    return static_cast<TComponent&>(*m_entity_storage[ComponentIDFactory::StaticID<TComponent>()]);
}

END_RUKEN_NAMESPACE