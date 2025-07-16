#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include "JobSystem/JobSystem.hpp"
#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/Tasks/Task.hpp"
#include "JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/DirectoryPath.hpp"
#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/Handlers/DebugHandler.hpp"
#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

#include "Windowing/WindowManager.hpp"
#include "Windowing/Window.hpp"

#include "Rendering/VulkanInstance.hpp"
#include "Rendering/Renderer.hpp"

#include <tracy/Tracy.hpp>

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

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
    Task<MainQueue> Run() const noexcept
    {
        // When the awaited primitive is signaled (in this case when the task is done),
        // the Run() coroutine is then scheduled back into the MainQueue, waiting to be picked up
        // by the first available thread.

        // Main loop
        for (int i = 0; i < 100; ++i)
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
 *
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop loop {"Loop", EntityAdmin {in_service_provider}};

    auto       const filesystem {in_service_provider.LocateService<Filesystem>()};
    FileHandle const file       {filesystem->Open(FilePath {
        .Directory = DirectoryPath {
            .Location = EFilesystemLocation::ProjectDirectory,
            .Path     = "Assets"
        },
        .Filename = "test.zip"
    })};

    RkSize        const filesize {file->GetFileSize()};
    std::vector<RkByte> buffer   {};

    buffer.reserve(filesize);
    for (int i = 0; i < 25; i++)
        file->Read(buffer.data(), {0, EFilePosition::Beginning}, buffer.capacity());

    co_await loop.Setup();
    co_await loop.Run  ();

    in_stop_source.request_stop();

    co_return;
}

/**
 * Initializes services and waits for the async main function to request a stop.
 *
 * @param in_argc Argument count
 * @param in_argv Argument values
 * @return Error code
 */
int main(int in_argc, char* in_argv[])
{
    // 1. --- Setup logging and general configuration. ---
    ConsoleHandler console_handler {};
    DebugHandler   debug_handler   {};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };
    std::initializer_list              queues   { &MainQueue::instance, &IOJobQueue::instance };

    MainQueue ::instance.SetMaximumConcurrency(8);
    IOJobQueue::instance.SetMaximumConcurrency(8);

    auto worker_bias_function = [](RkUint64 const in_total, RkUint64 const in_current, JobSystem& in_job_system) -> BinaryTreePath {
        return {
            .path  = in_current < 3ULL ? 0b1ULL : 0b0ULL,
            .depth = 1
        }; // We simply let all threads try to distribute themselves fairly among all queues
    };

    std::vector<const RkChar*> vulkan_layers     {};
    std::vector<const RkChar*> vulkan_extensions {};

    // 2. --- Initializing services and core systems. ---
    ServiceProvider    services   {"Root"};
    Logger*            logger     {services.ProvideService<Logger   >(handlers)};
    JobSystem*         job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};
    WindowsFilesystem* filesystem {services.ProvideService<WindowsFilesystem>("..")};
    WindowManager*     windowing  {services.ProvideService<WindowManager>()};
    VulkanInstance*    vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    Renderer*          renderer   {services.ProvideService<Renderer>()};

    // 3. --- Running async main. ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    auto window {windowing->CreateWindow(Constants<Vector2px>::standard_definition, "Coucou")};

    // And waiting for it to complete as a worker.
    job_system->CallerAsWorker(stop_source.get_token(), "CPU Main");

    return 0;
}