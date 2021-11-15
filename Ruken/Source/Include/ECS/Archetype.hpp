
#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "Build/Namespace.hpp"

#include "Meta/Tag.hpp"

#include "ECS/Group.hpp"
#include "ECS/Range.hpp"
#include "ECS/Entity.hpp"
#include "ECS/ComponentBase.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Archetypes are at the very core of this ECS implementation.
 *        They are responsible for storing and organizing the components
 *        so that systems can query and work on those in the most efficient way.
 *
 * An archetype is a unique set of arrays, each array being used to store one (and one only) type of component.
 * All these arrays are ordered so that all components at a given index correspond to the same entity;
 * and so, all entities stored in a given archetype share the exact same set of components.
 * By storing the components in contiguous homogeneous arrays, the systems can iterate on them very efficiently,
 * leveraging the hardware pre-fetcher to its fullest potential.
 *
 * \note This has a very important implication: each time the structure of an entity is modified
 *       (i.e. each time we add or remove a component to an entity), it must be memmoved to another archetype.
 *       This has a cost, especially if doing this on lots of entities very frequently.
 *       Take this fact into consideration when designing your code.
 */
class Archetype
{
    protected:

        #pragma region Members

        ArchetypeFingerprint m_fingerprint      {};
        std::list<Range>     m_free_entities    {};
        RkSize               m_entities_count   {0ULL};
        RkSize               m_free_space_count {0ULL};

        std::unordered_map<RkSize, std::unique_ptr<ComponentBase>> m_components {};

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Returns a free entity location by either looking up for a free spot, or allocating a new one 
         * \return Free entity location
         */
        RkSize GetFreeEntityLocation() noexcept;

        #pragma endregion 

    public:

        #pragma region Constructors

        template <ComponentType... TComponents>
        Archetype(Tag<TComponents...>) noexcept;

        Archetype(Archetype const& in_copy) = default;
        Archetype(Archetype&&      in_move) = default;
        ~Archetype()                        = default;

        #pragma endregion

        #pragma region Methods

        // Getters
        [[nodiscard]] std::list<Range>     const& GetFreeEntitiesRanges() const noexcept;
        [[nodiscard]] ArchetypeFingerprint const& GetFingerprint       () const noexcept;
        [[nodiscard]] RkSize                      GetEntitiesCount     () const noexcept;

        /**
         * \brief Returns a component of the passed type stored in this archetype
         * \tparam TComponent Component to look for
         * \note Passing a component type that does not exists in this archetype will result in a crash
         * \return Found component
         */
        template<ComponentType TComponent>
        [[nodiscard]]
        TComponent& GetComponent() noexcept;

        /**
         * \brief Creates an entity in the archetype
         * \return Entity handle.
         * \see Entity for lifetime info
         * \note Make sure to reinitialize your components after creating a new entity since the memory is pooled and thus
         *       almost never de-allocated. New memory will be allocated only if the archetype has no more empty spaces to fill
         */
        [[nodiscard]]
        Entity CreateEntity() noexcept;

        /**
         * \brief Deletes an entity from the archetype
         * \param in_local_identifier Local identifier of the entity, if invalid, this method does nothing
         */
        RkVoid DeleteEntity(RkSize in_local_identifier) noexcept;

        /**
         * \brief Creates a components reference group
         * \tparam TComponents Components to include in the group
         * \return Newly created reference group
         */
        template <ComponentType... TComponents>
        [[nodiscard]]
        Group<TComponents...> CreateGroupReference() noexcept;

        #pragma endregion

        #pragma region Operators

        Archetype& operator=(Archetype const& in_copy) = default;
        Archetype& operator=(Archetype&&      in_move) = default;

        #pragma endregion
};

#include "ECS/Archetype.inl"

END_RUKEN_NAMESPACE
