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

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

#define RUKEN_IO_QUEUE MainQueue

#include "IO/File.hpp"
#include "IO/IOJobQueue.hpp"

USING_RUKEN_NAMESPACE

struct AsyncLoop
{
    const char* name;
    EntityAdmin scene;

    Task<MainQueue> Setup()
    {
        scene.CreateSystem<CounterSystem>();
        for (int i = 0; i < 10'000'000; i++)
            scene.CreateEntity<CounterComponent>();

        co_await scene.ExecuteEvent(EEventName::OnStart);
    }

    [[nodiscard]]
    Task<MainQueue> Run() noexcept
    {
        // When the awaited primitive is signaled (in this case when the task is done),
        // the Run() coroutine is then scheduled back into the MainQueue, waiting to be picked up
        // by the first available thread.

        // Main loop
        for (int i = 0; i < 2000; ++i)
        {
            co_await scene.ExecuteEvent(EEventName::OnStart);
            File::on_io_pull.Signal();

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

Task<MainQueue> Pull(std::stop_source& in_stop_source)
{
    if (in_stop_source.stop_requested())
        co_return;

    File::on_io_pull.Signal();
    Pull(in_stop_source);

    co_return;
}

/**
 * Asynchronous main.
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop loop {"Loop", EntityAdmin {in_service_provider}};

    co_await loop.Setup();
    co_await loop.Run  ();

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
    // 1. --- Setup logging and general configuration. ---
    ConsoleHandler console_handler {};
    DebugHandler   debug_handler   {{}};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };
    std::initializer_list              queues   { &MainQueue::instance, &IOJobQueue::instance };

    MainQueue::instance.SetMaximumConcurrency(4);

    auto worker_bias_function = [](RkUint64 in_total, RkUint64 in_current, JobSystem& in_job_system) -> RkUint64 {
        // Selects the queue bias of workers

        return 0; // For now, we always select the first queue.
    };

    // 2. --- Initializing services and core systems. ---
    ServiceProvider services   {"Root"};
    Logger*         logger     {services.ProvideService<Logger   >(handlers)};
    JobSystem*      job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};

    // 3. --- Running async main. ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    // And waiting for it to complete as a worker.
    job_system->CallerAsWorker(stop_source.get_token(), "CPU Main");

    return 0;
}