#include "ECS/System.hpp"
#include "ECS/EntityAdmin.hpp"
#include "ECS/EventHandlerBase.hpp"

#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

USING_RUKEN_NAMESPACE

EntityAdmin::EntityAdmin(ServiceProvider& in_service_provider) noexcept:
    Service {in_service_provider}
{ }

CPUDynamicTask<RkVoid> EntityAdmin::ExecuteEvent(EEventName const in_event_name) const noexcept
{
    for (auto const& system: m_systems)
        if (auto const handler = system->GetEventHandler(in_event_name))
            co_await handler->Execute();

    co_return;
}
