#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Core/Service.hpp"
#include "JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"

#include "ECS/Entity.hpp"
#include "ECS/System.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/EEventName.hpp"

#include "ECS/Safety/SystemType.hpp"
#include "ECS/Safety/AnyComponentType.hpp"
#include "ECS/Safety/ExclusiveComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Universes are for isolation.
 *        Each universe can be described as a simulation containing entities
 *        and a group of systems (rules) to maintain and update these entities.
 */
struct Universe final: Service
{
    #pragma region Lifetime

    explicit Universe(ServiceProvider& in_service_provider) noexcept;
    Universe		   (Universe const&) = delete;
    Universe		   (Universe&&     ) = delete;
	Universe& operator=(Universe const&) = delete;
	Universe& operator=(Universe&&     ) = delete;
	~Universe() override				 = default;

    #pragma endregion

    #pragma region Methods

    // --- Simulation manipulation

    /**
     * @brief Starts the execution of an event type
     * @param in_event_name Event type to execute
     */
    DynamicTask<> ExecuteEvent(EEventName in_event_name) const noexcept;

    // --- Entity / Systems lifetime manipulation

    /**
     * @brief Creates a system and adds it to the world
     * @tparam TSystem System type to push to the entity admin
     */
    template <SystemType TSystem>
    RkVoid CreateSystem() noexcept;

    /**
     * @brief Creates a new entity with given components
     * @tparam TComponents Components to attach to the new entity
     * @return Created entity id
     */
    template <AnyComponentType... TComponents>
    Entity CreateEntity() noexcept;

    /**
     * @brief Returns an exclusive component or instantiate it if needed
     * @tparam TComponent Component to access
     * @return Exclusive component reference
     */
    template <ExclusiveComponentType TComponent>
    TComponent& GetExclusiveComponent() noexcept;

    #pragma endregion

	private:

		#pragma region Members

		std::vector<std::unique_ptr<System>>                       m_systems              {};
		std::unordered_map<RkSize, std::unique_ptr<ComponentBase>> m_exclusive_components {};
		std::unordered_map<ArchetypeFingerprint, std::unique_ptr<Archetype>> m_archetypes {};

		#pragma endregion

		#pragma region Methods

		/**
		 * @brief Creates a new archetype and handles any setup co-routine
		 * @tparam TComponents Component types
		 */
		template <AnyComponentType... TComponents>
		Archetype* CreateArchetype() noexcept;

		#pragma endregion
};

#include "ECS/Universe.inl"

END_RUKEN_NAMESPACE
