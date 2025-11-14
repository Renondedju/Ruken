#include "Rendering.hpp"
#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"

#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/DebugHandler.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"

#include "Core/Maths/Vector/PixelVector2.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/STD/StdFilesystem.hpp"

#include "Resources/Assets/AssetImporter.hpp"
#include "Resources/ResourceManager.hpp"

#include "Rendering/Vulkan/VulkanInstance.hpp"
#include "Rendering/Windowing/Window.hpp"
#include "Rendering/SlangImporter.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/ShaderModule.hpp"
#include "Rendering/SpirvLoader.hpp"

#include "Queues.hpp"

USING_RUKEN_NAMESPACE

/**
 * Asynchronous main.
 *
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider const& in_service_provider)
{
    Logger    const* logger        {in_service_provider.LocateService<Logger>         ()};
    ResourceManager* resources     {in_service_provider.LocateService<ResourceManager>()};
    RenderDevice*    render_device {in_service_provider.LocateService<RenderDevice>   ()};

    try {

        // Loading pipeline code
        auto const code {resources->Request<ShaderModule>(FilePath {
            .location = EFilesystemLocation::ProjectDirectory,
            .path     = "slang.spv",
        })};

        Window window  {*render_device, Constants<Vector2px>::standard_definition, "Coucou"};
        TestWindowRenderer const test_window_renderer {
            .owner    = *render_device,
            .window   = window,
            .pipeline = code
        };

        co_await code.LoadEvent();

        while (!window.ShouldClose())
        {
            FrameMark;

            glfwPollEvents();
            co_await test_window_renderer.RenderFrame();
        }

    } catch (Exception& in_exception) {
        if (logger) logger->Exception("", "Async main has been interrupted : {}", in_exception.reason);
    } catch (std::exception& in_exception) {
        if (logger) logger->Exception("", "Async main has been interrupted : {}", in_exception.what());
    } catch (...) {
        if (logger) logger->Exception("", "Async main has been interrupted : Unknown exception occurred");
    }

    in_stop_source.request_stop();

    co_return;
}

/// @brief Initializes services and waits for the async main function to request a stop.
int main([[maybe_unused]] int   in_arg_count,
         [[maybe_unused]] char* in_arg_values[])
{
    // 1. --- Pre-initialization & Configuration ---
    SingleThreadSingleQueueExecutor main_executor   {MainQueue::instance};
    ConsoleHandler                  console_handler {};
    DebugHandler                    debug_handler   {};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };
    std::initializer_list              queues   { &ProcessingQueue::instance, &IOJobQueue::instance };

    auto worker_bias_function = [](RkUint64 const, RkUint64 const in_current, JobSystem&) {
        return BinaryTreePath {
            .path  = in_current < 3ULL ? 0b1ULL : 0b0ULL,
            .depth = 1
        }; // The first 3 threads will prioritize the IO queue.
    };

    std::vector<const RkChar*> vulkan_layers     {};
    std::vector<const RkChar*> vulkan_extensions {
        vk::KHRGetSurfaceCapabilities2ExtensionName,
        vk::EXTSurfaceMaintenance1ExtensionName
    };

    // 2. --- Initializing services and core systems ---
    ServiceProvider   services {"Application"};
    auto* logger     {services.ProvideService<Logger>(handlers)};
    auto* job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};
    auto* filesystem {services.ProvideService<StdFilesystem>("../Assets")};
    auto* vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    auto* renderer   {services.ProvideService<RenderDevice>()};
    auto* importer   {services.ProvideService<AssetImporter>()};
    auto* resources  {services.ProvideService<ResourceManager>()};

    importer ->ProvideImporter<SlangImporter>(); // TODO: Not used or working yet. Slang API is whack.
    resources->ProvideLoader  <SpirvLoader>  ();

    // 3. --- Finally, running async main ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    // ... and waiting for it to complete as a worker.
    main_executor.CallerAsWorker(stop_source.get_token(), "CPU Main");

    return EXIT_SUCCESS;
}