#include "ECS/System.hpp"
#include "ECS/Universe.hpp"
#include "ECS/SystemEventHandler.hpp"

#include <ranges>

USING_RUKEN_NAMESPACE

Universe::Universe(ServiceProvider& in_service_provider) noexcept:
    Service {in_service_provider, typeid(Universe)}
{ }

auto& Universe::ListArchetypes() noexcept
{
    return m_archetypes;
}

DynamicTask<> Universe::ExecuteEvent(EEventName const in_event_name) noexcept
{
    for (System const& system: m_systems)
    {
        if (SystemEventHandler* handler {system->GetEventHandler(in_event_name)})
        {
            handler->

        }
    }
}
