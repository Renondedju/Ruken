#include "Core/Kernel.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

#include "Core/Exception.hpp"
#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    const char* name;
    EntityAdmin domain;

    CPUDynamicTask<> SometimesThrows()
    {
        if (rand() % 100 == 0)
            throw Exception("Random exception");

        co_return;
    }

    [[nodiscard]]
    CPUDynamicTask<> Run() noexcept
    {
        domain.CreateSystem<CounterSystem>();
        for (int i = 0; i < 10'000'000; i++)
            domain.CreateEntity<CounterComponent>();

        co_await domain.ExecuteEvent(EEventName::OnStart);

        for (int i = 0; i < 2000; ++i)
        {
            co_await domain.ExecuteEvent(EEventName::OnStart);

            SometimesThrows();

            FrameMark;
            FrameMarkNamed(name);
        }

        co_await domain.ExecuteEvent(EEventName::OnEnd);
    }
};

CPUTask<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop loop {
        .name   = "Game loop",
        .domain = EntityAdmin {in_service_provider}
    };

    try
    { co_await loop.Run(); }
    catch (Exception& in_exception)
    {
        std::string const what {in_exception};
        TracyMessageC(what.c_str(), what.length(), 0xFF0000);
    }

    in_stop_source.request_stop();
}

int main(int in_argc, char* in_argv[])
{
    CentralProcessingUnit cpu      {};
    ServiceProvider       services {};

    std::stop_source stop_source {};

    AsyncMain(stop_source, services);

    cpu.RegisterQueue(MainQueue::instance);
    cpu.StartWorkers ();

    cpu.CallerAsWorker(stop_source.get_token());

    return 0;
}
