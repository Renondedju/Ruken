#pragma once

template <IsSystemEventHandler TEventHandler>
RkVoid System::SetupEventHandler(EECSEvents in_events) noexcept
{
    in_events.Foreach([&](EECSEventName in_event_name) {
        m_handlers.insert_or_assign(in_event_name, std::move(std::make_unique<TEventHandler>()));
    });
}