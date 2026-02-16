// Engine includes
#include "Core/Time/Clock.hpp"
#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"
#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/DebugHandler.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"
#include "Core/Maths/Vector/PixelVector.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/STD/StdFilesystem.hpp"

#include "Resources/Assets/AssetImporter.hpp"
#include "Resources/ResourceManager.hpp"

#include "Rendering/RenderDevice.hpp"
#include "Rendering/Vulkan/VulkanInstance.hpp"
#include "Rendering/Windowing/Window.hpp"
#include "Rendering/Resources/SlangImporter.hpp"
#include "Rendering/Resources/ShaderModule.hpp"
#include "Rendering/Resources/SpirvLoader.hpp"
#include "Rendering/Resources/ObjLoader.hpp"
#include "Rendering/Resources/GPUMesh.hpp"

// App includes
#include "Rendering.hpp"
#include "Universe.hpp"
#include "Systems/ApplyTransformHandler.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : QueueHandle<MainQueue, 64>{};

/**
 * Asynchronous main.
 *
 * @param in_service_provider Service Provider.
 */
AsyncTask<MainQueue> AsyncMain(ServiceProvider const& in_service_provider) noexcept
{
    // --- 1. Init
    Clock  const*    clock         {in_service_provider.LocateService<Clock>          ()};
    ResourceManager* resources     {in_service_provider.LocateService<ResourceManager>()};
    RenderDevice*    render_device {in_service_provider.LocateService<RenderDevice>   ()};
    Universe*        universe      {in_service_provider.LocateService<Universe>       ()};

    auto const code {resources->Request<ShaderModule>(FilePath
        { .location = EFilesystemLocation::ProjectDirectory, .path = "slang.spv"   })};
    auto const mesh {resources->Request<GPUMesh     >(FilePath
        { .location = EFilesystemLocation::ProjectDirectory, .path = "suzanne.obj" })};

    Window             window  {*render_device, Constants<Vector2px>::standard_definition, "Coucou"};
    TestWindowRenderer test_window_renderer {
        .owner    = *render_device,
        .window   = window,
        .pipeline = code,
        .mesh     = mesh
    };

    // Waiting for resources
    co_await WhenAll(mesh.LoadEvent(), code.LoadEvent());

    // --- 2. Start
    FrameMark;
    co_await universe->ExecuteEvent(EECSEventName::OnStart);

    // --- 3. Main Loop
    while (!window.ShouldClose())
    {
        FrameMark;

        glfwPollEvents();
        co_await universe->ExecuteEvent(EECSEventName::OnUpdate);
        co_await test_window_renderer.RenderFrame(clock->TimeSinceCreation());
    }

    // --- 4. Cleanup
    co_await universe->ExecuteEvent(EECSEventName::OnEnd);

    co_return;
}

ECSTask<RkVoid> AsyncTestMain(ServiceProvider const& in_service_provider) noexcept
{
    SharedMutex<int> value {};

    co_await ParallelFor(0uz, 100uz, [&](RkSize in_index) {
       return value.AsyncWrite();
    });

    co_return;
}

/**
 * Runs async main and logs any caught exceptions.
 *
 * @param in_stop_source Program stop source.
 * @param in_service_provider Service Provider.
 */
AsyncTask<MainQueue> TryCatchAsyncMain(std::stop_source& in_stop_source, ServiceProvider const& in_service_provider)
{
    Logger const* logger {in_service_provider.LocateService<Logger>()};

    try {
        co_await AsyncMain(in_service_provider);
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
    std::set_terminate(&TerminateHandler);

    SingleThreadSingleQueueExecutor main_executor   {MainQueue::instance};
    ConsoleHandler                  console_handler {};
    DebugHandler                    debug_handler   {};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };
    std::initializer_list              queues   { &ECSJobQueue::instance, &IOJobQueue::instance };

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
    auto* clock      {services.ProvideService<Clock>()};
    auto* logger     {services.ProvideService<Logger>(handlers)};
    auto* job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};
    auto* filesystem {services.ProvideService<StdFilesystem>("../Assets")};
    auto* vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    auto* renderer   {services.ProvideService<RenderDevice>()};
    auto* importer   {services.ProvideService<AssetImporter>()};
    auto* resources  {services.ProvideService<ResourceManager>()};
    auto* universe   {services.ProvideService<Universe>()};

    importer ->ProvideImporter<SlangImporter> (); // TODO: Not used or working yet. Slang API is whack.
    resources->ProvideLoader  <SpirvLoader>   ();
    resources->ProvideLoader  <ObjLoader>     ();

    universe->CreateEntities<Position, Rotation, Scale, Transform>(10'000'000);
    universe->CreateSystem  <MoveSystem>();
    universe->CreateSystem  <ApplyTransform>();

    // 3. --- Finally, running async main ---
    std::stop_source  stop_source {};
    TryCatchAsyncMain(stop_source, services);

    // ... and waiting for it to complete as a worker.
    main_executor.CallerAsWorker(stop_source.get_token(), "CPU Main");

    return EXIT_SUCCESS;
}