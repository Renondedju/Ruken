#pragma once

#include <memory>
#include <unordered_map>

#include "ECS/EEventName.hpp"
#include "ECS/SystemEventHandler.hpp"

BEGIN_RUKEN_NAMESPACE

struct Universe;

/**
 * \brief Systems transform data. They implement the logic that modifies the components.
 * \note Systems are a way to logically order event handlers, to treat a particular type of data
 * \see System::EventHandler
 *
 * It is important to notice that for lots of actions, you don�t care about the specific type of an entity;
 * what you care about is specific properties of these entities.
 * E.g. for rendering all you need is a mesh and a transform matrix; you don�t care if the entity is a player or a tree.
 */
class System
{
    public:

        // Helpers, allows for shorter declarations later on
        // template <IsComponent... TComponents> using UpdateEventHandler = EventHandler<EEventName::OnUpdate, TComponents...>;
        // template <IsComponent... TComponents> using StartEventHandler  = EventHandler<EEventName::OnStart , TComponents...>;
        // template <IsComponent... TComponents> using EndEventHandler    = EventHandler<EEventName::OnEnd   , TComponents...>;

    private:

        #pragma region Members

        // Event handlers, used for the update of the system
        std::unordered_map<EEventName, std::unique_ptr<SystemEventHandler>> m_handlers {};

        #pragma endregion

    protected:

        #pragma region Members

        Universe& m_universe;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Setups an event handler in the system.
         *        Note that only one event handler for each event type is currently handled,
         *        setting multiple ones will simply override the previous one
         * \tparam TEventHandler Event handler type, must inherit from System::EventHandler
         */
        template <EventHandlerType TEventHandler>
        RkVoid SetupEventHandler() noexcept;

        #pragma endregion

    public:

        #pragma region Lifetime

        explicit System(Universe& in_universe) noexcept;
        System& operator=(System const&) = delete;
        System& operator=(System&&     ) = delete;
        System           (System const&) = default;
        System           (System&&     ) = default;
        virtual ~System()                = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Binds the passed archetype with the setuped event handlers of that system
         * \param in_archetype Archetype to bind
         */
        RkVoid BindArchetype(Archetype& in_archetype) noexcept;

        /**
         * \brief Returns the setup event handler for the passed event (if any) 
         * \param in_event_name Event name
         * \return Event handler instance or nullptr
         */
        SystemEventHandler* GetEventHandler(EEventName in_event_name) const noexcept;

        #pragma endregion
};

#include "ECS/System.inl"

END_RUKEN_NAMESPACE