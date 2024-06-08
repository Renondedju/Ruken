
#pragma once

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"

#include "ECS/ArchetypeFingerprint.hpp"
#include "ECS/Safety/AnyComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief A component query allows to quickly check if an archetype matches a given query
 *        This is used by systems to find archetypes to iterate over and process
 */
class ComponentQuery
{
    private:

        #pragma region Members

        ArchetypeFingerprint m_included {};
        ArchetypeFingerprint m_excluded {};

        #pragma endregion

    public:

        #pragma region Constructors

        ComponentQuery()                              = default;
        ComponentQuery(ComponentQuery const& in_copy) = default;
        ComponentQuery(ComponentQuery&&      in_move) = default;
        ~ComponentQuery()                             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Setups the inclusion query of the group.
         *        Passed components will be *required* by the query.
         * \tparam TComponents Required components of the query
         */
        template <AnyComponentType... TComponents>
        RkVoid SetupInclusionQuery() noexcept;

        /**
         * \brief Setups the exclusion query of the group.
         *        Any entity owning the passed components will be *excluded* by the query,
         *        even if the inclusion query selected them in the first place.
         * \tparam TComponents Excluded components of the query
         */
        template <AnyComponentType... TComponents>
        RkVoid SetupExclusionQuery() noexcept;

        /**
         * \brief Checks if the passed archetype matches the query
         * \param in_archetype Archetype to match
         * \return True if the query matched, false otherwise
         */
        RkBool Match(Archetype const& in_archetype) const noexcept;

        #pragma endregion

        #pragma region Operators

        ComponentQuery& operator=(ComponentQuery const& in_copy) = default;
        ComponentQuery& operator=(ComponentQuery&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ComponentQuery.inl"

END_RUKEN_NAMESPACE