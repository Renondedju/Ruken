#include "Core/Time/Clock.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"
#include "Core/JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"
#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"
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

// App includes
#include "Application.hpp"
#include "Rendering.hpp"
#include "Universe.hpp"
#include "Systems/ApplyTransformHandler.hpp"

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
	auto const* clock      {root_services.LocateService <Clock>()};
	auto*       filesystem {root_services.ProvideService<StdFilesystem>("../Assets")};
	auto*       vulkan     {root_services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
	auto*       renderer   {root_services.ProvideService<RenderDevice>()};
	auto*       importer   {root_services.ProvideService<AssetImporter>()};
	auto*       resources  {root_services.ProvideService<ResourceManager>()};
	auto*       universe   {root_services.ProvideService<Universe>()};

	importer ->ProvideImporter<SlangImporter> (); // TODO: Not used or working yet. Slang API is whack.
	resources->ProvideLoader  <SpirvLoader>   ();
	resources->ProvideLoader  <ObjLoader>     ();

	//universe->CreateEntities<Position, Rotation, Scale, Transform>(100'000);
	//universe->CreateSystem  <ApplyTransform>();
	//universe->CreateSystem  <MoveSystem>();
	//universe->CreateSystem  <ApplyTransform2>();

	auto const code {resources->Request<ShaderModule>(FilePath
		{ .location = EFilesystemLocation::ProjectDirectory, .path = "slang.spv"   })};
	auto const mesh {resources->Request<GPUMesh     >(FilePath
		{ .location = EFilesystemLocation::ProjectDirectory, .path = "suzanne.obj" })};

	Window             window  {*renderer, Constants<Vector2px>::standard_definition, "Coucou"};
	TestWindowRenderer test_window_renderer {
		.owner    = *renderer,
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

AsyncTask<MainQueue> Application::AsyncTestMain()
{


	co_return;
}
