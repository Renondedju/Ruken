#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Core/Service.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"

#include "ECS/Entity.hpp"
#include "ECS/ECSTask.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/EECSEventName.hpp"
#include "ECS/Systems/System.hpp"
#include "ECS/Components/UniverseComponent.hpp"

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
     * @warning Only one event must be ran at once for a single universe.
     * @param in_event_name Event type to execute
     */
    ECSTask<RkVoid> ExecuteEvent(EECSEventName in_event_name) noexcept;

    // --- Entity / Systems lifetime manipulation

    /**
     * @brief Creates a system and adds it to the world
     * @tparam TSystem System type to push to the entity admin
     */
    template <IsSystem TSystem>
    RkVoid CreateSystem() noexcept;

    /**
     * @brief Creates a new entity with given components
     * @tparam TComponents Components to attach to the new entity
     * @return Created entity id
     */
    template <IsComponent... TComponents>
    RkVoid CreateEntities(RkSize in_count) noexcept;

    /**
     * @brief Returns an exclusive component or instantiate it if needed
     * @tparam TComponent Component to access
     * @return Exclusive component reference
     */
    template <IsUniverseComponent TComponent>
    TComponent& GetUniverseComponent() noexcept;

    #pragma endregion

	private:

		#pragma region Members

		std::vector       <						 std::unique_ptr<System   >> m_systems    {};
		std::unordered_map<ComponentFingerprint, std::unique_ptr<Component>> m_components {};
		std::unordered_map<ComponentFingerprint, std::unique_ptr<Archetype>> m_archetypes {};

		#pragma endregion

		#pragma region Methods

		/**
		 * @brief Creates a new archetype and handles any setup co-routine
		 * @tparam TComponents Component types
		 */
		template <IsComponent... TComponents>
		Archetype* CreateArchetype() noexcept;

		#pragma endregion
};

#include "ECS/Universe.inl"

END_RUKEN_NAMESPACE
