#include "Core/Kernel.hpp"
#include "JobSystem/CPU/CentralProcessingUnit.hpp"
#include "JobSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "JobSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "JobSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    const char* name;
    EntityAdmin domain;

    [[nodiscard]]
    CPUDynamicTask<> Run() noexcept
    {
        domain.CreateSystem<CounterSystem>();
        for (int i = 0; i < 1'000'000; i++)
            domain.CreateEntity<CounterComponent>();

        co_await domain.ExecuteEvent(EEventName::OnStart);

        for (int i = 0; i < 500000; ++i)
        {
            co_await domain.ExecuteEvent(EEventName::OnUpdate);

            FrameMark;
        }

        co_await domain.ExecuteEvent(EEventName::OnEnd);
    }
};

CPUTask<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    in_stop_source.request_stop();

    co_return;
}

int main(int i_argc, char* i_argv[])
{
    CentralProcessingUnit cpu      {};
    ServiceProvider       services {};

    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    cpu.RegisterQueue(MainQueue::instance);
    cpu.StartWorkers();
    cpu.CallerAsWorker(stop_source.get_token());

    return 0;
}
