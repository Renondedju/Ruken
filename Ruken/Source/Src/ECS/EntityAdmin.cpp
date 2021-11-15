
#include "ECS/EntityAdmin.hpp"
#include "Core/ServiceProvider.hpp"

USING_RUKEN_NAMESPACE

RkVoid EntityAdmin::BuildUpdatePlan() noexcept
{
    m_update_plan.ResetPlan();

    for (auto& system: m_systems)
    {
        m_update_plan.AddInstruction([&system] { system->OnUpdate(); });
        m_update_plan.EndInstructionPack();
    }
}

EntityAdmin::EntityAdmin(ServiceProvider& in_service_provider) noexcept:
    Service     {in_service_provider},
    m_scheduler {m_service_provider.LocateService<Scheduler>()}
{
    // The entity admin requires a scheduler to be able to work
    if (!m_scheduler)
        SignalServiceInitializationFailure("The entity admin requires a scheduler to be able to work, updates are asynchronous");
}

RkVoid EntityAdmin::StartSimulation() noexcept
{
    // Simulation start is synchronous for now
    for (auto& system: m_systems)
        system->OnStart();
}

RkVoid EntityAdmin::UpdateSimulation() noexcept
{
    m_update_plan.ExecutePlanAsynchronously(*m_scheduler);
}

RkVoid EntityAdmin::EndSimulation() noexcept
{
    // Simulation end is synchronous for now
    for (auto& system: m_systems)
        system->OnEnd();
}
