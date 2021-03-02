
#include "ECS/EntityAdmin.hpp"
#include "Core/ServiceProvider.hpp"

#include "ECS/System.hpp"

USING_RUKEN_NAMESPACE

EntityAdmin::EntityAdmin(ServiceProvider& in_service_provider) noexcept:
    Service     {in_service_provider},
    m_scheduler {m_service_provider.LocateService<Scheduler>()}
{ }

RkVoid EntityAdmin::BuildEventExecutionPlan(EEventName const in_event_name) noexcept
{
    std::unique_ptr<ExecutionPlan>& execution_plan = m_execution_plans[in_event_name];

    if (execution_plan == nullptr)
        execution_plan = std::make_unique<ExecutionPlan>();

    execution_plan->ResetPlan();

    // FIXME: This current implementation does not take care of the potential optimizations that could be made
    // by taking into account component read and writes
    for (auto& system: m_systems)
    {
        EventHandlerBase* event_handler = system->GetEventHandler(in_event_name);
        if (event_handler == nullptr)
            continue;

        execution_plan->AddInstruction([&event_handler] { event_handler->Execute(); });
        execution_plan->EndInstructionPack();
    }
}

RkVoid EntityAdmin::ExecuteEvent(EEventName const in_event_name) noexcept
{
    if (m_execution_plans[in_event_name] == nullptr)
        BuildEventExecutionPlan(in_event_name);

    std::unique_ptr<ExecutionPlan>& execution_plan = m_execution_plans.at(in_event_name);

    // If multithreading is available
    if (m_scheduler)
        execution_plan->ExecutePlanAsynchronously(*m_scheduler);
    else
        execution_plan->ExecutePlanSynchronously();
}
