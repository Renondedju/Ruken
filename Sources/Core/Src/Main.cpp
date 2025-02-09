#include "Core/Kernel.hpp"
#include "Core/Exception.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    const char* name;
    EntityAdmin scene;

    CPUTask<MainQueue> SometimesThrows()
    {
        if (rand() % 100 == 0)
            throw Exception("Random exception");

        co_return;
    }

    [[nodiscard]]
    CPUTask<MainQueue> Run() noexcept
    {
        scene.CreateSystem<CounterSystem>();
        for (int i = 0; i < 10'000'000; i++)
            scene.CreateEntity<CounterComponent>();

        co_await scene.ExecuteEvent(EEventName::OnStart);
        // ^^^   At this point the function is paused, and an awaiter is attached
        //       to the synchronisation primitive returned by the invocation of the task.

        // When the awaited primitive is signaled (in this case when the task is done),
        // the Run() coroutine is then scheduled back into the MainQueue, waiting to be picked up
        // by the first available thread.

        // Main loop
        for (int i = 0; i < 500; ++i)
        {
            co_await scene.ExecuteEvent(EEventName::OnStart);

            FrameMark;
        }

        co_await scene.ExecuteEvent(EEventName::OnEnd);
    }
};

CPUTask<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop loop {
        .name   = "Game loop",
        .scene = EntityAdmin {in_service_provider}
    };

    try
    {
        co_await loop.Run();
    }
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