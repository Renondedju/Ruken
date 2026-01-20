#include "System.hpp"
#include "SystemEventHandler.hpp"

#include <ranges>

USING_RUKEN_NAMESPACE

System::System(Universe& in_universe) noexcept:
    m_universe {in_universe}
{ }

SystemEventHandler* System::GetEventHandler(EECSEventName const in_event_name) const noexcept
{
    if (m_handlers.contains(in_event_name))
        return m_handlers.at(in_event_name).get();

    return nullptr;
}
