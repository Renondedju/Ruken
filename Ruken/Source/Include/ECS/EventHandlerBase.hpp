#pragma once

#include "Build/Namespace.hpp"

#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include "ECS/ComponentQuery.hpp"
#include "ECS/EEventName.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Base class of every event handler
 *        Used to store handlers by reference or pointer without having to deal with templates
 */
class EventHandlerBase
{
    protected:

        ComponentQuery m_query {};

    public:

        #pragma region Lifetime

        EventHandlerBase()                        = default;
        EventHandlerBase(EventHandlerBase const&) = default;
        EventHandlerBase(EventHandlerBase&&     ) = default;
        virtual ~EventHandlerBase()               = default;
    
        EventHandlerBase& operator=(EventHandlerBase const&) = default;
        EventHandlerBase& operator=(EventHandlerBase&&     ) = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the component query of the event handler
         * \return Component query
         */
        ComponentQuery const& GetQuery() const noexcept;

        /**
         * \brief Returns the name of the handled event
         * \return Event name
         */
        virtual EEventName GetHandledEvent() noexcept = 0;

        /**
         * \brief Adds an archetype reference to the event handler.
         * \note The passed archetype must be compatible with the event handler component query
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        virtual RkVoid AddArchetypeReference(Archetype& in_archetype) noexcept = 0;

        /**
         * \brief Runs the event handler
         */
        virtual CPUDynamicTask<RkVoid> Execute() noexcept = 0;

        #pragma endregion
};

END_RUKEN_NAMESPACE