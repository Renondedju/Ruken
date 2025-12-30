#include "ECS/System.hpp"
#include "ECS/SystemEventHandler.hpp"

#include <ranges>

USING_RUKEN_NAMESPACE

System::System(Universe& in_universe) noexcept:
    m_universe {in_universe}
{ }

RkVoid System::BindArchetype(Archetype& in_archetype) noexcept
{
    for(auto const& event_handler: m_handlers | std::views::values)
        if (event_handler->GetQuery().Match(in_archetype))
            event_handler->AddArchetypeReference(in_archetype);
}

SystemEventHandler* System::GetEventHandler(EEventName const in_event_name) const noexcept
{
    if (m_handlers.contains(in_event_name))
        return m_handlers.at(in_event_name).get();

    return nullptr;
}
