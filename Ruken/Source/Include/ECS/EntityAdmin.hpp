
#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Build/Namespace.hpp"

#include "Core/Service.hpp"

#include "ECS/Entity.hpp"
#include "ECS/System.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/EEventName.hpp"
#include "ECS/SystemBase.hpp"

#include "Threading/Scheduler.hpp"
#include "Threading/ExecutionPlan.hpp"

#include "ECS/Safety/SystemType.hpp"
#include "ECS/Safety/ComponentType.hpp"
#include "ECS/Safety/ExclusiveComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief EntityAdmins are for isolation.
 *        Each admin can be described as a simulation containing entities
 *        and a group of system to maintain and update theses entities.
 */
class EntityAdmin final: public Service<EntityAdmin>
{
    private:

        #pragma region Members

        std::vector<std::unique_ptr<System>>                       m_systems              {};
        std::unordered_map<RkSize, std::unique_ptr<ComponentBase>> m_exclusive_components {};
        std::unordered_map<EEventName, std::unique_ptr<ExecutionPlan>>  m_execution_plans {};
        std::unordered_map<ArchetypeFingerprint, std::unique_ptr<Archetype>> m_archetypes {};

        Scheduler* m_scheduler {nullptr};

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Creates a new archetype and handles any setup co-routine
         * \tparam TComponents Component types
         */
        template <ComponentType... TComponents>
        Archetype* CreateArchetype() noexcept;

        #pragma endregion 

    public:

        #pragma region Constructors

        EntityAdmin(ServiceProvider& in_service_provider) noexcept;

        EntityAdmin(EntityAdmin const& in_copy) = delete;
        EntityAdmin(EntityAdmin&&      in_move) = delete;
        ~EntityAdmin() override                 = default;

        #pragma endregion

        #pragma region Methods

        // --- Simulation manipulation

        /**
         * \brief Builds an event execution plan
         * \param in_event_name Event type to build the execution plan for
         */
        RkVoid BuildEventExecutionPlan(EEventName in_event_name) noexcept;

        /**
         * \brief Starts the execution of an event type
         * \param in_event_name Event type to execute
         */
        RkVoid ExecuteEvent(EEventName in_event_name) noexcept;

        // --- Entity / Systems lifetime manipulation

        /**
         * \brief Creates a system and adds it to the world
         * \tparam TSystem System type to push to the entity admin 
         */
        template <SystemType TSystem>
        RkVoid CreateSystem() noexcept;

        /**
         * \brief Creates a new entity with given components
         * \tparam TComponents Components to attach to the new entity
         * \return Created entity id
         */
        template <ComponentType... TComponents>
        Entity CreateEntity() noexcept;

        /**
         * \brief Returns an exclusive component or instantiate it if needed
         * \tparam TComponent Component to access
         * \return Exclusive component reference
         */
        template <ExclusiveComponentType TComponent>
        TComponent& GetExclusiveComponent() noexcept;

        #pragma endregion

        #pragma region Operators

        EntityAdmin& operator=(EntityAdmin const& in_copy) = delete;
        EntityAdmin& operator=(EntityAdmin&&      in_move) = delete;

        #pragma endregion
};

#include "ECS/EntityAdmin.inl"

END_RUKEN_NAMESPACE
