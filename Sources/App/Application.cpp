#include "Core/Time/Clock.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"
#include "Core/JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp"
#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Maths/Vector/PixelVector.hpp"

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
#include "Rendering/Shaders/CameraData.hpp"
#include "Rendering/Shaders/GridParameters.hpp"

// App includes
#include "Application.hpp"

#include "CameraController.hpp"
#include "Rendering.hpp"
#include "Universe.hpp"
#include "Simulation/Flock.hpp"

USING_RUKEN_NAMESPACE

AsyncTask<MainQueue> Application::TryCatchAsyncMain(std::stop_source& in_stop_source)
{
	Logger const* logger {root_services.LocateService<Logger>()};

	try {
		co_await AsyncMain();
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

AsyncTask<MainQueue> Application::AsyncMain()
{
	std::vector<const RkChar*> vulkan_layers     {};
	std::vector<const RkChar*> vulkan_extensions {
		vk::KHRGetSurfaceCapabilities2ExtensionName,
		vk::EXTSurfaceMaintenance1ExtensionName
	};

	// --- 1. Init
	auto* clock      {root_services.LocateService <Clock		   >()};
	auto* filesystem {root_services.ProvideService<StdFilesystem   >("../Assets")};
	auto* vulkan     {root_services.ProvideService<VulkanInstance  >(vulkan_layers, vulkan_extensions)};
	auto* renderer   {root_services.ProvideService<RenderDevice    >()};
	auto* importer   {root_services.ProvideService<AssetImporter   >()};
	auto* resources  {root_services.ProvideService<ResourceManager >()};
	auto* window     {root_services.ProvideService<Window		   >(Constants<Vector2px>::standard_definition, "Boids Demo")};
	auto* controller {root_services.ProvideService<CameraController>()};

	// Initializing resources
	importer ->ProvideImporter<SlangImporter> (); // TODO: Not used or working yet. Slang API is whack.
	resources->ProvideLoader  <SpirvLoader>   ();
	resources->ProvideLoader  <ObjLoader>     ();

	auto const grid_code {resources->Request<SpirvCode>(FilePath {
		.location = EFilesystemLocation::ProjectDirectory,
		.path     = "Grid.spv"
	})};
	auto const boid_code {resources->Request<SpirvCode>(FilePath {
		.location = EFilesystemLocation::ProjectDirectory,
		.path     = "Basic.spv"
	})};
	auto const boid_mesh {resources->Request<GPUMesh  >(FilePath {
		.location = EFilesystemLocation::ProjectDirectory,
		.path     = "arrow.obj"
	})};

	Flock flock {10'000};

	// Waiting for resources
	co_await WhenAll(
		grid_code.LoadEvent(),
		boid_code.LoadEvent(),
		boid_mesh.LoadEvent()
	);

	TestWindowRenderer test_window_renderer {
		.owner      = *renderer,
		.window     = *window,
		.boid_code  = *boid_code.Current(),
		.grid_code  = *grid_code.Current(),
		.mesh       = boid_mesh,
		.controller = *controller,
		.flock      = flock,
	};

	// --- 2. Main Loop
	while (!window->ShouldClose())
	{
		FrameMark;
		clock->Tick();

		glfwPollEvents();
		co_await flock.Update(root_services);
		co_await test_window_renderer.RenderFrame(clock->TimeSinceCreation());
	}

	// --- 3. Cleanup
	co_return;
}
