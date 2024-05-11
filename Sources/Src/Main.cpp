#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include <functional>
#include <tracy/Tracy.hpp>

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    std::string                                                  name;
    std::function<CPUDynamicTask<void>(AsyncLoop const& i_this)> init;
    std::function<CPUDynamicTask<bool>(AsyncLoop const& i_this)> loop;

    [[nodiscard]]
    CPUDynamicTask<> Run() const
    {
        co_await init(*this);

        while(co_await loop(*this))
        {
            FrameMark;
            TracyCFrameMarkNamed(name.c_str());
        }
    }
};

CPUTask<MainQueue> AsyncMain(std::stop_source& i_stop_source)
{
    AsyncLoop const loop {
        .name = "Game loop",
        .init = [](auto const& i_this) -> CPUDynamicTask<void> { co_return; },
        .loop = [](auto const& i_this) -> CPUDynamicTask<bool> { static int a {};  co_return (a++) < 10000; },
    };

    AsyncLoop const client_loop {
        .name = "Client loop",
        .init = [](auto const& i_this) -> CPUDynamicTask<void> { co_return; },
        .loop = [](auto const& i_this) -> CPUDynamicTask<bool> { static int a {};  co_return (a++) < 10000; },
    };

    co_await WhenAll({client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), client_loop.Run(), loop.Run()});

    i_stop_source.request_stop();
}

int main()
{
    CentralProcessingUnit cpu      {};
    ServiceProvider       services {};

    std::stop_source stop_source {};
    AsyncMain(stop_source);

    cpu.RegisterQueue(MainQueue::instance);
    cpu.CallerAsWorker(stop_source.get_token());

    return 0;
}
