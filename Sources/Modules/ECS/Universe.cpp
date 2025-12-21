#include "ECS/System.hpp"
#include "ECS/Universe.hpp"
#include "ECS/EventHandlerBase.hpp"

#include "JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"

USING_RUKEN_NAMESPACE

Universe::Universe(ServiceProvider& in_service_provider) noexcept:
    Service {in_service_provider, typeid(Universe)}
{ }

DynamicTask<> Universe::ExecuteEvent(EEventName const in_event_name) const noexcept
{
    for (auto const& system: m_systems)
        if (auto const handler = system->GetEventHandler(in_event_name))
            co_await handler->Execute();
}
