#pragma once

#include <memory>
#include <unordered_map>

#include "ECS/EECSEventName.hpp"
#include "ECS/Systems/SystemEventHandler.hpp"

BEGIN_RUKEN_NAMESPACE

struct Universe;

/**
 * @brief Systems transform data. They implement the logic that modifies the components.
 * @note Systems are a way to logically order event handlers, to treat a particular type of data
 * @see System::EventHandler
 *
 * It is important to notice that for lots of actions, you don�t care about the specific type of an entity;
 * what you care about is specific properties of these entities.
 * E.g. for rendering all you need is a mesh and a transform matrix; you don�t care if the entity is a player or a tree.
 */
class System
{
    #pragma region Members

    // Event handlers, used for the update of the system
    std::unordered_map<EECSEventName, std::unique_ptr<SystemEventHandler>> m_handlers {};

    #pragma endregion

    protected:

        #pragma region Members

        Universe& m_universe;

        #pragma endregion

        #pragma region Methods

        /**
         * @brief Setups an event handler in the system.
         * @param in_events Event
         * @tparam TEventHandler Event handler type
         */
        template <IsSystemEventHandler TEventHandler>
        RkVoid SetupEventHandler(EECSEvents in_events) noexcept;

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
         * \brief Returns the setup event handler for the passed event (if any) 
         * \param in_event_name Event name
         * \return Event handler instance or nullptr
         */
        SystemEventHandler* GetEventHandler(EECSEventName in_event_name) const noexcept;

        #pragma endregion
};

#include "System.inl"

template <typename TType>
concept IsSystem = std::is_base_of_v<System, TType>;

END_RUKEN_NAMESPACE
