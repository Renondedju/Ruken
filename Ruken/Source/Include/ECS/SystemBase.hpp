
#pragma once

#include "Build/Namespace.hpp"

#include "ECS/EEventName.hpp"
#include "ECS/ComponentQuery.hpp"
#include "ECS/EventHandlerBase.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class SystemBase
{
    public:

        #pragma region Members


        #pragma endregion

        #pragma region Constructors

        SystemBase()                          = default;
        SystemBase(SystemBase const& in_copy) = default;
        SystemBase(SystemBase&&      in_move) = default;
        virtual ~SystemBase()                 = default;

        #pragma endregion

        #pragma region Methods

        // --- Virtual

        /**
         * \brief Adds a component reference group to the system.
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        virtual RkVoid AddReferenceGroup(Archetype& in_archetype) noexcept = 0;

        #pragma endregion

        #pragma region Operators

        SystemBase& operator=(SystemBase const& in_copy) = default;
        SystemBase& operator=(SystemBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE
