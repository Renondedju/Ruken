#include "Core/Kernel.hpp"
#include "Core/Exception.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"

#include <tracy/Tracy.hpp>
#include <functional>

#include "ExecutiveSystem/CPU/Awaitables/Primitives/SharedMutex.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    const char* name;
    EntityAdmin scene;

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
        for (int i = 0; i < 200; ++i)
        {
            co_await scene.ExecuteEvent(EEventName::OnStart);

            FrameMark;
        }

        co_await scene.ExecuteEvent(EEventName::OnEnd);
    }
};

CPUTask<MainQueue> Read(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncRead();
    TracyMessageL("Reading !");
}

CPUTask<MainQueue> Write(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncWrite();
    TracyMessageL("Writing !");
    (*access)++;
};

/**
 * Asynchronous main
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider
 */
CPUTask<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    SharedMutex<RkInt64> mutex {};

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto const vector = {
        Write(mutex),
        Read (mutex),
        Read (mutex),
        Read (mutex),
        Write(mutex),
        Write(mutex),
        Write(mutex)
    };
    co_await WhenAll<CPUTask<MainQueue>>(vector);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    in_stop_source.request_stop();

    co_return;
}

/**
 * Initializes services and waits for the async main function to request a stop.
 * @param in_argc Argument count
 * @param in_argv Argument values
 * @return Error code
 */
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