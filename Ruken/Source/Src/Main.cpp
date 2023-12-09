#include "Core/ServiceProvider.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

#include "Utility/Benchmark.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

USING_RUKEN_NAMESPACE

int main()
{
    // Declaring the context
    CentralProcessingUnit job_system       {};
    ServiceProvider       service_provider {};

    // Initializing services
    std::string failure_reason {};

    Logger*      logger       {service_provider.ProvideService<Logger     >(failure_reason, "Ruken")};
    EntityAdmin* entity_admin {service_provider.ProvideService<EntityAdmin>(failure_reason)};

    entity_admin->CreateSystem<CounterSystem>();

    for (int index = 0; index < 6553500 / 2; ++index)
        entity_admin->CreateEntity<CounterComponent>();

    for (int index = 0; index < 6553500 / 2; ++index)
        entity_admin->CreateEntity<CounterComponent, TestTagComponent>();

    RUKEN_BENCHMARK("OnStart")
        entity_admin->ExecuteEvent(EEventName::OnStart);    
}
