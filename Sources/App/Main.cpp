#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/Tasks/Task.hpp"

#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/DebugHandler.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"

#include "Core/Maths/Vector/PixelVector2.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/STD/StdFilesystem.hpp"
#include "JobSystem/Awaitables/Primitives/WhenAll.hpp"

#include "Resources/Assets/AssetImporter.hpp"
#include "Resources/ResourceManager.hpp"

#include "Rendering/Vulkan/VulkanInstance.hpp"
#include "Rendering/Windowing/Window.hpp"
#include "Rendering/SlangImporter.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/ShaderModule.hpp"
#include "Rendering/SpirvLoader.hpp"
#include "Rendering/GPUFence.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"
#include "Rendering/Resources/GPUImageView.hpp"
#include "Rendering/Coroutines/GPUTask.hpp"
#include "Rendering/Coroutines/GPUPromise.hpp"

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

USING_RUKEN_NAMESPACE

struct SwapchainImage
{
    // This pointer needs to be kept alive for the image and view to stay valid.
    ResourcePtr<GPUSwapchainData> owner;

    vk::ImageView view;
    vk::Image     image;
};

GPUTask DrawTriangle(
    RenderDevice&,
    SwapchainImage const& in_swapchain_image,
    vk::Viewport   const& in_viewport,
    vk::Extent2D   const& in_extent,
    vk::Pipeline   const& in_pipeline)
{
    /*
     * co_await image.TransitionLayout({
     *      ...
     * });
     */
    vk::ImageMemoryBarrier2 const barrier {
        .srcStageMask   = vk::PipelineStageFlagBits2::eTopOfPipe,
        .srcAccessMask  = {},
        .dstStageMask   = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        .dstAccessMask  = vk::AccessFlagBits2       ::eColorAttachmentWrite,
        .oldLayout      = vk::ImageLayout::eUndefined,
        .newLayout      = vk::ImageLayout::eColorAttachmentOptimal,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = in_swapchain_image.image,
        .subresourceRange    = {
            .aspectMask     = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        }
    };

    GPU::command_buffer->pipelineBarrier2(vk::DependencyInfo {
        .dependencyFlags         = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers    = &barrier
    });
    // --

    /*
     * Attachments described as a "return" value ?
     * RenderTask::promise_type would be in charge of generating the various attachment infos.
     *
     * RenderTask<Image<Format::eRGBA>> Render();
     * RenderTask                       Render(Output<Image> out_image);
     *
     */
    vk::RenderingAttachmentInfo const attachment_info {
        .imageView   = in_swapchain_image.view,
        .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
        .loadOp      = vk::AttachmentLoadOp::eClear,
        .storeOp     = vk::AttachmentStoreOp::eStore,
        .clearValue  = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
    };

    GPU::command_buffer->beginRendering(vk::RenderingInfo {
        .renderArea = {
            .offset = { 0, 0 },
            .extent = in_extent
        },
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &attachment_info
    });

    // Actual coroutine code
    GPU::command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, in_pipeline);
    GPU::command_buffer->setViewport (0, in_viewport);
    GPU::command_buffer->setScissor  (0, vk::Rect2D(vk::Offset2D(0, 0), in_extent));
    GPU::command_buffer->draw        (3, 1, 0, 0);
    GPU::command_buffer->endRendering();

    /*
     * co_await image.TransitionLayout({
     *      ...
     * });
     */
    vk::ImageMemoryBarrier2 const barrier2 {
        .srcStageMask   = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        .srcAccessMask  = vk::AccessFlagBits2       ::eColorAttachmentWrite,
        .dstStageMask   = vk::PipelineStageFlagBits2::eBottomOfPipe,
        .dstAccessMask  = {},
        .oldLayout      = vk::ImageLayout::eColorAttachmentOptimal,
        .newLayout      = vk::ImageLayout::ePresentSrcKHR,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = in_swapchain_image.image,
        .subresourceRange    = {
            .aspectMask     = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        }
    };

    GPU::command_buffer->pipelineBarrier2(vk::DependencyInfo {
        .dependencyFlags         = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers    = &barrier2
    });

    co_return;
}

Task<MainQueue> RenderFrame(RenderDevice& in_render_device, Window& in_window, ResourceHandle<ShaderModule> const in_program)
{
    auto         const& pipeline_ptr  {in_program              .Current()};
    auto         const& swapchain_ptr {in_window.GetSwapchain().Current()};
    vk::Extent2D const  extent        {in_window.GetExtent()};
    vk::Viewport const  viewport      {
        .x        = 0.0f, .y        = 0.0f,
        .width    = static_cast<float>(extent.width),
        .height   = static_cast<float>(extent.height),
        .minDepth = 0.0f, .maxDepth = 1.0f
    };

    GPUFence const acquire_image {in_render_device.GetDevice(), vk::FenceCreateInfo { .flags = {} }};

    auto [result, imageIndex] = swapchain_ptr->swapchain.acquireNextImage(UINT64_MAX, nullptr, acquire_image.fence);

    acquire_image.WaitSynchronously();

    co_await DrawTriangle(in_render_device, SwapchainImage {
        .owner = swapchain_ptr,
        .view  = swapchain_ptr->images_views         [imageIndex],
        .image = swapchain_ptr->swapchain.getImages()[imageIndex],
    }, viewport, extent, pipeline_ptr->pipeline);

    std::ignore = in_render_device.GetQueue().presentKHR(vk::PresentInfoKHR {
        .waitSemaphoreCount = 0,
        .pWaitSemaphores    = nullptr,
        .swapchainCount     = 1,
        .pSwapchains        = &*swapchain_ptr->swapchain,
        .pImageIndices      = &imageIndex,
        .pResults           = nullptr
    });

    co_return;
}

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

        Window window {*render_device, Constants<Vector2px>::standard_definition, "Coucou"};

        co_await code.LoadEvent();

        while (!window.ShouldClose())
        {
            FrameMark;
            glfwPollEvents();

            const RkChar* description {nullptr};
            if (glfwGetError(&description) != GLFW_NO_ERROR)
                throw Exception("GLFW Error: " + std::string(description));
            
            co_await RenderFrame(*render_device, window, code);
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

    // 2. --- Initializing services and core systems ---
    ServiceProvider    services   {"Application"};
    Logger*            logger     {services.ProvideService<Logger>(handlers)};
    JobSystem*         job_system {services.ProvideService<JobSystem>(queues, worker_bias_function)};
    StdFilesystem*     filesystem {services.ProvideService<StdFilesystem>("../Assets")};
    VulkanInstance*    vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    RenderDevice*      renderer   {services.ProvideService<RenderDevice>()};
    AssetImporter*     importer   {services.ProvideService<AssetImporter>()};
    ResourceManager*   resources  {services.ProvideService<ResourceManager>()};

    importer ->ProvideImporter<SlangImporter>(); // TODO: Not used or working yet. Slang API is whack.
    resources->ProvideLoader  <SpirvLoader>  ();

    // 3. --- Finally, running async main ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    // ... and waiting for it to complete as a worker.
    job_system->CallerAsWorker(stop_source.get_token(), "CPU Main");

    return EXIT_SUCCESS;
}