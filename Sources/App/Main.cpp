#include "SpirvLoader.hpp"
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

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

USING_RUKEN_NAMESPACE

/**
 * Asynchronous main.
 *
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider const& in_service_provider)
{
    Logger          const* logger        {in_service_provider.LocateService<Logger>         ()};
    AssetImporter   const* importer      {in_service_provider.LocateService<AssetImporter>  ()};
    ResourceManager*       resources     {in_service_provider.LocateService<ResourceManager>()};
    RenderDevice*          render_device {in_service_provider.LocateService<RenderDevice>   ()};

    try {
        //co_await importer->Import(FilePath {
        //    .location = EFilesystemLocation::ProjectDirectory,
        //    .path     = "test.slang"
        //});

        // Loading pipeline code
        auto const code {resources->Request<ShaderModule>(FilePath {
            .location = EFilesystemLocation::ProjectDirectory,
            .path     = "slang.spv",
        })};

        Window window {*render_device, Constants<Vector2px>::standard_definition, "Coucou"};

        vk::raii::Semaphore     present_complete_semaphore {render_device->GetDevice(), vk::SemaphoreCreateInfo()};
        vk::raii::Semaphore     render_finished_semaphore  {render_device->GetDevice(), vk::SemaphoreCreateInfo()};
        vk::raii::Fence         draw_fence                 {render_device->GetDevice(), {.flags = vk::FenceCreateFlagBits::eSignaled}};
        vk::raii::SwapchainKHR& swapchain {window.GetSwapchain()};
        vk::Extent2D     		extent    {window.GetExtent()};
        vk::Viewport     		viewport  {
            .x        = 0.0f,
            .y        = 0.0f,
            .width    = static_cast<float>(extent.width),
            .height   = static_cast<float>(extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        co_await code.LoadTask();

        while (!window.ShouldClose())
        {
            FrameMark;
            glfwPollEvents();
            
            auto [result, imageIndex] = swapchain.acquireNextImage(UINT64_MAX, *present_complete_semaphore, nullptr);

            vk::raii::CommandBuffer& command_buffer {render_device->GetCommandBuffer()};

            command_buffer.begin({});

            vk::ImageMemoryBarrier2 barrier {
                .srcStageMask   = vk::PipelineStageFlagBits2::eTopOfPipe,
                .srcAccessMask  = {},
                .dstStageMask   = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dstAccessMask  = vk::AccessFlagBits2       ::eColorAttachmentWrite,
                .oldLayout      = vk::ImageLayout::eUndefined,
                .newLayout      = vk::ImageLayout::eColorAttachmentOptimal,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image               = swapchain.getImages()[imageIndex],
                .subresourceRange    = {
                    .aspectMask     = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            command_buffer.pipelineBarrier2(vk::DependencyInfo {
                .dependencyFlags         = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers    = &barrier
            });

            vk::RenderingAttachmentInfo attachmentInfo {
                .imageView   = window.GetViews()[imageIndex],
                .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
                .loadOp      = vk::AttachmentLoadOp::eClear,
                .storeOp     = vk::AttachmentStoreOp::eStore,
                .clearValue  = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
            };

            command_buffer.beginRendering(vk::RenderingInfo {
                .renderArea = {
                    .offset = { 0, 0 },
                    .extent = extent
                },
                .layerCount           = 1,
                .colorAttachmentCount = 1,
                .pColorAttachments    = &attachmentInfo
            });

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, (*code)->pipeline);
            command_buffer.setViewport(0, viewport);
            command_buffer.setScissor (0, vk::Rect2D(vk::Offset2D(0, 0), extent));
            command_buffer.draw(3, 1, 0, 0);
            command_buffer.endRendering();

            vk::ImageMemoryBarrier2 barrier2 {
                .srcStageMask   = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .srcAccessMask  = vk::AccessFlagBits2       ::eColorAttachmentWrite,
                .dstStageMask   = vk::PipelineStageFlagBits2::eBottomOfPipe,
                .dstAccessMask  = {},
                .oldLayout      = vk::ImageLayout::eColorAttachmentOptimal,
                .newLayout      = vk::ImageLayout::ePresentSrcKHR,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image               = swapchain.getImages()[imageIndex],
                .subresourceRange    = {
                    .aspectMask     = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            command_buffer.pipelineBarrier2(vk::DependencyInfo {
                .dependencyFlags         = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers    = &barrier2
            });

            command_buffer.end();
            render_device->GetDevice().resetFences(*draw_fence);

            vk::PipelineStageFlags wait_destination_stage_mask { vk::PipelineStageFlagBits::eColorAttachmentOutput };
            vk::SubmitInfo         submit_info {
                .waitSemaphoreCount   = 1,
                .pWaitSemaphores      = &*present_complete_semaphore,
                .pWaitDstStageMask    = &wait_destination_stage_mask,
                .commandBufferCount   = 1,
                .pCommandBuffers      = &*command_buffer,
                .signalSemaphoreCount = 1,
                .pSignalSemaphores    = &*render_finished_semaphore
            };

            render_device->GetQueue().submit(submit_info, *draw_fence);

            while (vk::Result::eTimeout == render_device->GetDevice().waitForFences(*draw_fence, vk::True, UINT64_MAX))
                ;

            auto a = render_device->GetQueue().presentKHR(vk::PresentInfoKHR {
                .waitSemaphoreCount = 1,
                .pWaitSemaphores    = &*render_finished_semaphore,
                .swapchainCount     = 1,
                .pSwapchains        = &*swapchain,
                .pImageIndices      = &imageIndex,
                .pResults           = nullptr
            });
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
    StdFilesystem*     filesystem {services.ProvideService<StdFilesystem>("../Assets")};
    VulkanInstance*    vulkan     {services.ProvideService<VulkanInstance>(vulkan_layers, vulkan_extensions)};
    RenderDevice*      renderer   {services.ProvideService<RenderDevice>()};
    AssetImporter*     importer   {services.ProvideService<AssetImporter>()};
    ResourceManager*   resources  {services.ProvideService<ResourceManager>()};

    importer ->ProvideImporter<SlangImporter>();
    resources->ProvideLoader  <SpirvLoader>  ();

    // 3. --- Running async main. ---
    std::stop_source stop_source {};
    AsyncMain(stop_source, services);

    // And waiting for it to complete as a worker.
    job_system->CallerAsWorker(stop_source.get_token(), "CPU Main");

    return 0;
}