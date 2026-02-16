#include "ECS/Universe.hpp"
#include "ECS/Systems/System.hpp"
#include "ECS/Systems/SystemEventHandler.hpp"

#include "Core/JobSystem/Awaitables/Primitives/ParallelForEach.hpp"

USING_RUKEN_NAMESPACE

Universe::Universe(ServiceProvider& in_service_provider) noexcept:
    Service {in_service_provider, typeid(Universe)}
{ }

ECSTask<RkVoid> Universe::ExecuteEvent(EECSEventName const in_event_name) noexcept
{
    std::vector<SyncTask<>> handlers {};
    for (std::unique_ptr<System> const& system: m_systems)
    {
        SystemEventHandler* handler {system->GetEventHandler(in_event_name)};
        if (!handler)
            continue;

        // !! Lock acquisition MUST be done synchronously to ensure proper execution ordering. !!
        for (auto& [fingerprint, archetype] : m_archetypes)
            if (handler->component_query({}, fingerprint))
                handlers.emplace_back(handler->ScheduleExecution(*this, *archetype.get()));
    }

    co_await WhenAll(handlers);
}
