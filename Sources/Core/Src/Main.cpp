#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include "JobSystem/JobSystem.hpp"
#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/Tasks/Task.hpp"
#include "JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/Handlers/DebugHandler.hpp"
#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

#include <tracy/Tracy.hpp>

USING_RUKEN_NAMESPACE

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

struct AsyncLoop
{
    const char* name;
    EntityAdmin scene;

    [[nodiscard]]
    Task<MainQueue> Run() noexcept
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
        for (int i = 0; i < 2000; ++i)
        {
            co_await scene.ExecuteEvent(EEventName::OnStart);

            FrameMark;
        }

        co_await scene.ExecuteEvent(EEventName::OnEnd);
    }
};

Task<MainQueue> Read(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncRead();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

Task<MainQueue> Write(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncWrite();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    (*access)++;
}

/**
 * Asynchronous main.
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop            loop  {"Loop", EntityAdmin {in_service_provider}};
    SharedMutex<RkInt64> mutex {};

    co_await WhenAll<Task<MainQueue>> ({
        Read (mutex),
        Read (mutex),
        Write(mutex),
        Read (mutex),
        Read (mutex),
        Read (mutex),
        Write(mutex),
        Write(mutex),
        Write(mutex),
        Read (mutex),
        Read (mutex),
        loop.Run()
    });

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
    // Setup logging
    ConsoleHandler console_handler {};
    DebugHandler   debug_handler   {{}};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };

    // Initializing services and core systems
    ServiceProvider services   {"Root"};
    Logger*         logger     {services.ProvideService<Logger>(handlers)};
    JobSystem*      job_system {services.ProvideService<JobSystem>()};

    std::stop_source stop_source {};

    // Pushing async main to the MainQueue
    AsyncMain(stop_source, services);

    // Starting workers
    job_system->RegisterQueue (MainQueue::instance);
    job_system->StartWorkers  ();
    job_system->CallerAsWorker(stop_source.get_token());

    return 0;
}