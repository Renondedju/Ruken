#include "Core/Kernel.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

#include "../ThirdParty/tracy/dede20a49fd55ec615451f07b969d950651359aa/public/tracy/Tracy.hpp"
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
    AsyncLoop loop2 {
        .name   = "Editor loop",
        .domain = EntityAdmin {in_service_provider}
    };
    AsyncLoop loop3 {
        .name   = "Test loop",
        .domain = EntityAdmin {in_service_provider}
    };
    AsyncLoop loop4 {
        .name   = "Test loop 2",
        .domain = EntityAdmin {in_service_provider}
    };
    AsyncLoop loop5 {
        .name   = "Test loop 3",<
        .domain = EntityAdmin {in_service_provider}
    };

    co_await WhenAll({loop.Run(), loop2.Run(), loop3.Run(), loop4.Run(), loop5.Run()});

    in_stop_source.request_stop();
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
