#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/QueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

CPUTask<MainQueue> TracyUpdate(std::stop_token const& in_stop_token) noexcept
{
    using namespace std::chrono_literals;

    while(!in_stop_token.stop_requested())
    {
        std::this_thread::sleep_for(100ms);

    }

    co_return;
}

struct AsyncLoop
{
    const char* name;
    EntityAdmin domain;

    [[nodiscard]]
    CPUDynamicTask<> Run() noexcept
    {
        domain.CreateSystem<CounterSystem>();
        for (int i = 0; i < 10'000'000; i++)
            domain.CreateEntity<CounterComponent>();

        co_await domain.ExecuteEvent(EEventName::OnStart);
        std::cout << "on start" << std::endl;

        for (int i = 0; i < 5000; ++i)
        {
            co_await domain.ExecuteEvent(EEventName::OnStart);

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

    co_await loop.Run();

    std::cout << "stop" << std::endl;

    in_stop_source.request_stop();
}

int main(int in_argc, char* in_argv[])
{
    CentralProcessingUnit cpu      {};
    ServiceProvider       services {};

    std::stop_source stop_source {};

    AsyncMain  (stop_source, services);
    //TracyUpdate(stop_source.get_token());

    cpu.RegisterQueue(MainQueue::instance);
    cpu.StartWorkers ();

    cpu.CallerAsWorker(stop_source.get_token());

    return 0;
}
