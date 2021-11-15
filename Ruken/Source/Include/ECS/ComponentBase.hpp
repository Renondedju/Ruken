
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief Base class of the Component class, this is simply used to store components as they might have templates
 */
class ComponentBase
{
    protected:

        #pragma region Members

        inline static RkSize m_id_counter {0ULL};

        // Owning archetypes are only required for components that live in archetypes
        // Witch isn't the case for exclusive components, that lives in the entity admin
        //
        // Components that requires an archetype to exist will only expose references
        Archetype const* m_owning_archetype {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype
         */
        ComponentBase(Archetype const* in_owning_archetype) noexcept;

        ComponentBase(ComponentBase const& in_copy) = default;
        ComponentBase(ComponentBase&&      in_move) = default;
        virtual ~ComponentBase()                    = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        [[nodiscard]]
        virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept = 0;

        #pragma endregion

        #pragma region Operators

        ComponentBase& operator=(ComponentBase const& in_copy) = default;
        ComponentBase& operator=(ComponentBase&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief Generates the code required to create a unique ID for any component
 */
#define RUKEN_DEFINE_COMPONENT_ID_DECLARATION inline static RkSize GetId() noexcept { static RkSize id = m_id_counter++; return id; }

END_RUKEN_NAMESPACE
