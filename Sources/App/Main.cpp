#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"

#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/DebugHandler.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"

#include "Core/Maths/Vector/PixelVector2.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Rendering/Vulkan/VulkanInstance.hpp"
#include "Rendering/Windowing/Window.hpp"
#include "Rendering/SlangImporter.hpp"
#include "Rendering/RenderDevice.hpp"

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

USING_RUKEN_NAMESPACE

IOTask<> LoadShader(ServiceProvider& in_service_provider)
{
    SlangImporter    importer    {};
    Filesystem*      filesystem  {in_service_provider.LocateService<Filesystem>()};
    FileHandle const shader_file {filesystem->Open(FilePath {
        .Filename  = "test.slang",
        .Directory = DirectoryPath {
            .Location = EFilesystemLocation::ProjectDirectory,
            .Path     = "."
        }
    })};

    co_await importer.Import(shader_file);
}

/**
 * Asynchronous main.
 *
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    Logger const* logger {in_service_provider.LocateService<Logger>()};

    try {
        co_await LoadShader(in_service_provider);
    } catch (Exception& in_exception) {
        if (logger) logger->Error("", "{}", in_exception.reason);
    } catch (std::exception& in_exception) {
        if (logger) logger->Error("", "{}", in_exception.what());
    }

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

    auto worker_bias_function = [](RkUint64 const _, RkUint64 const in_current, JobSystem& __) {
        return BinaryTreePath {
            .path  = in_current < 3ULL ? 0b1ULL : 0b0ULL,
            .depth = 1
        }; // The first 3 threads will prioritize the IO queue.
    };

    std::vector<const RkChar*> vulkan_layers     {};
    std::vector<const RkChar*> vulkan_extensions {};

    // 2. --- Initializing services and core systems. ---
    ServiceProvider    services   {"Root"};
    Logger*            logger     {services.ProvideService<Logger>(handlers)};
    JobSystem*         job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};
    WindowsFilesystem* filesystem {services.ProvideService<WindowsFilesystem>("../Assets")};
    VulkanInstance*    vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    RenderDevice*      renderer   {services.ProvideService<RenderDevice>()};

    // 3. --- Running async main. ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    Window window {*renderer, Constants<Vector2px>::standard_definition, "Coucou"};

    // And waiting for it to complete as a worker.
    job_system->CallerAsWorker(stop_source.get_token(), "CPU Main");

    return 0;
}