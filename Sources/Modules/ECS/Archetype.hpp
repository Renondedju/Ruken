#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "Core/Meta/Empty.hpp"

#include "ECS/Range.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/EntityComponent.hpp"
#include "ECS/Components/ArchetypeComponent.hpp"
#include "ECS/Components/ComponentFingerprint.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Archetypes are at the very core of this ECS implementation.
 *        They are responsible for storing and organizing the components
 *        so that systems can query and work on those in the most efficient way.
 *
 * An archetype is a unique set of arrays, each array being used to store one type of component.
 * All these arrays are ordered so that all components at a given index correspond to the same entity;
 * and so, all entities stored in a given archetype share the exact same set of components.
 * By storing the components in contiguous homogeneous arrays, the systems can iterate on them very efficiently,
 * leveraging the hardware pre-fetcher to its fullest potential.
 *
 * @note This has a very important implication: each time the structure of an entity is modified
 *       (i.e. each time we add or remove a component to an entity), it must be memmoved to another archetype.
 *       This has a cost, especially if doing this on lots of entities very frequently.
 *       Take this fact into consideration when designing your code.
 */
class Archetype
{
    protected:

        #pragma region Members

        ComponentFingerprint m_fingerprint;
        RkSize               m_entities_per_chunk;
        RkSize               m_entities_count     {0ULL};

        // Component storage, only holds entity components
        std::unordered_map<ComponentID, std::unique_ptr<ArchetypeComponent>> m_entity_storage {};

        #pragma endregion 

    public:

        #pragma region Lifetime

        template <IsArchetypeComponent... TComponents>
        explicit Archetype(Empty<TComponents...>) noexcept;
        Archetype& operator=(Archetype const&) = delete;
        Archetype& operator=(Archetype&&     ) = delete;
        Archetype           (Archetype const&) = delete;
        Archetype           (Archetype&&     ) = delete;
        ~Archetype()                           = default;

        #pragma endregion

        #pragma region Methods

        // Getters
        [[nodiscard]] RkSize                      GetChunkSize    () const noexcept;
        [[nodiscard]] RkSize                      GetEntitiesCount() const noexcept;
        [[nodiscard]] ComponentFingerprint const& GetFingerprint  () const noexcept;

        /**
         * @brief Returns a component of the passed type stored in this archetype
         * @tparam TComponent Component to look for
         * @note Passing a component type that does not exist in this archetype will result in a crash
         * @return Found component
         */
        template<IsEntityComponent TComponent>
        [[nodiscard]] TComponent& GetComponent() noexcept;

        /**
         * @brief Creates a set amount of entities in the archetype
         * @see Entity for lifetime info
         * @note Make sure to reinitialize your components after creating a new entity since the memory is pooled and thus
         *       almost never de-allocated. New memory will be allocated only if the archetype has no more empty spaces to fill
         */
        RkVoid CreateEntities(RkSize in_count = 1) noexcept;

        /**
         * @brief Deletes an entity from the archetype
         * @param in_local_identifier Local identifier of the entity, if invalid, this method does nothing
         */
        RkVoid DeleteEntity(RkSize in_local_identifier) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE

#include "ECS/Archetype.inl"