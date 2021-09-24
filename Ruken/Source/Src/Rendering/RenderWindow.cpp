#include "Rendering/RenderWindow.hpp"

#include "Debug/Logging/Logger.hpp"

#include "Rendering/RenderContext.hpp"
#include "Rendering/RenderDevice.hpp"

#include "Windowing/Window.hpp"

#include "Meta/Safety.hpp"

#include "Rendering/Renderer.hpp"

#include "Rendering/Resources/Model.hpp"
#include "Rendering/Resources/Texture.hpp"

USING_RUKEN_NAMESPACE

static std::unique_ptr<Model> g_model;

RkVoid RenderWindow::CreateSurface(Window const& in_window) noexcept
{
    VkSurfaceKHR surface;

    vk::Result const result = static_cast<vk::Result>(glfwCreateWindowSurface(m_context->GetInstance(), in_window.GetHandle(), nullptr, &surface));

    if (result == vk::Result::eSuccess)
        m_surface = surface;
    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create surface!"))
}

RkVoid RenderWindow::CreateSwapchain(vk::SwapchainKHR in_old_swapchain) noexcept
{
    auto [result, value] = m_device->GetPhysicalDevice().getSurfaceSupportKHR(0U, m_surface);

    if (!value)
        return;
        
    vk::SwapchainCreateInfoKHR swapchain_create_info = {
        .surface               = m_surface,
        .minImageCount         = m_image_count,
        .imageFormat           = m_image_format,
        .imageColorSpace       = m_color_space,
        .imageExtent           = m_image_extent,
        .imageArrayLayers      = 1U,
        .imageUsage            = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode      = vk::SharingMode::eExclusive,
        .queueFamilyIndexCount = 0U,
        .pQueueFamilyIndices   = {},
        .preTransform          = vk::SurfaceTransformFlagBitsKHR::eIdentity,
        .compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode           = m_present_mode,
        .clipped               = VK_TRUE,
        .oldSwapchain          = in_old_swapchain
    };

    std::tie(result, m_swapchain) = m_device->GetLogicalDevice().createSwapchainKHR(swapchain_create_info);

    if (result == vk::Result::eSuccess)
    {
        m_valid = true;

        std::tie(result, m_images) = m_device->GetLogicalDevice().getSwapchainImagesKHR(m_swapchain);

        for (auto const& image : m_images)
        {
            vk::ImageViewCreateInfo image_view_info = {
                .image = image,
                .viewType = vk::ImageViewType::e2D,
                .format = m_image_format,
                .subresourceRange = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .levelCount = 1,
                    .layerCount = 1
                }
            };

            m_image_views.push_back(m_device->GetLogicalDevice().createImageView(image_view_info).value);

            std::vector attachments = {
                m_image_views.back(),
                m_depth_view
            };

            vk::FramebufferCreateInfo framebuffer_create_info = {
                .renderPass = Renderer::render_pass,
                .attachmentCount = static_cast<uint32_t>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = m_image_extent.width,
                .height = m_image_extent.height,
                .layers = 1
            };

            m_framebuffers.push_back(m_device->GetLogicalDevice().createFramebuffer(framebuffer_create_info).value);
        }
    }

    else
        m_valid = false;

    if (in_old_swapchain)
        m_device->GetLogicalDevice().destroySwapchainKHR(in_old_swapchain);
}

RkVoid RenderWindow::PickSwapchainImageCount()
{
    auto [result, capabilities] =  m_device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);

    m_image_count = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && m_image_count > capabilities.maxImageCount)
        m_image_count = capabilities.maxImageCount;
}

RkVoid RenderWindow::PickSwapchainExtent(RkInt32 const in_width, RkInt32 const in_height)
{
    auto [result, capabilities] =  m_device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);

    if (capabilities.currentExtent.width != UINT32_MAX)
        m_image_extent = capabilities.currentExtent;
    else
    {
        m_image_extent.width  = std::clamp(static_cast<RkUint32>(in_width),  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
        m_image_extent.height = std::clamp(static_cast<RkUint32>(in_height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
}

RkVoid RenderWindow::PickSwapchainFormat()
{
    auto [result, formats] = m_device->GetPhysicalDevice().getSurfaceFormatsKHR(m_surface);

    m_image_format = formats[0].format;
    m_color_space  = formats[0].colorSpace;
}

RkVoid RenderWindow::PickSwapchainPresentMode()
{
    auto [result, present_modes] =  m_device->GetPhysicalDevice().getSurfacePresentModesKHR(m_surface);

    m_present_mode = present_modes[0];
}

RenderWindow::RenderWindow(Window& in_window, RenderContext* in_context, RenderDevice* in_device, Logger* in_logger) noexcept:
    m_logger  {in_logger},
    m_context {in_context},
    m_device  {in_device}
{
    CreateSurface(in_window);
    PickSwapchainImageCount();
    PickSwapchainExtent(in_window.GetFramebufferSize().width, in_window.GetFramebufferSize().height);
    PickSwapchainFormat();
    PickSwapchainPresentMode();

    vk::AllocationCreateInfo allocation_info = {
        .usage = vk::MemoryUsage::eGpuOnly
    };

    vk::ImageCreateInfo image_create_info = {
        .imageType = vk::ImageType::e2D,
        .format = vk::Format::eD32Sfloat,
        .extent = {
            .width = m_image_extent.width,
            .height = m_image_extent.height,
            .depth = 1
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment
    };


    auto [result, value] = m_device->GetAllocator().createImage(image_create_info, allocation_info);

    std::tie(m_depth, m_depth_allocation) = value;

    vk::ImageViewCreateInfo depth_view_info = {
        .image = m_depth,
        .viewType = vk::ImageViewType::e2D,
        .format = vk::Format::eD32Sfloat,
        .subresourceRange = {
            .aspectMask = vk::ImageAspectFlagBits::eDepth,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    std::tie(result, m_depth_view) = m_device->GetLogicalDevice().createImageView(depth_view_info);

    CreateSwapchain();

    in_window.on_framebuffer_resized.Subscribe([this] (RkInt32 const in_width, RkInt32 const in_height)
    {
        PickSwapchainExtent(in_width, in_height);
        CreateSwapchain(m_swapchain);
    });

    in_window.on_closed.Subscribe([this] ()
    {
        m_valid = false;
    });

    for (RkUint32 i = 0U; i < 2U; ++i)
    {
        m_render_frames.emplace_back(m_device, m_logger);
    }

    g_model = std::make_unique<Model>(m_device, "Data/viking_room.obj");
}

RenderWindow::~RenderWindow() noexcept
{
    g_model.reset();

    m_device->GetAllocator().destroyImage(m_depth, m_depth_allocation);

    if (m_swapchain)
        m_device->GetLogicalDevice().destroySwapchainKHR(m_swapchain);

    if (m_surface)
        m_context->GetInstance().destroySurfaceKHR(m_surface);
}

RkUint32 RenderWindow::GetImageCount() const noexcept
{
    return m_image_count;
}

vk::Format RenderWindow::GetImageFormat() const noexcept
{
    return m_image_format;
}

vk::Extent2D RenderWindow::GetImageExtent() const noexcept
{
    return m_image_extent;
}

std::vector<vk::Image> const& RenderWindow::GetImages() const noexcept
{
    return m_images;
}

std::vector<vk::ImageView> const& RenderWindow::GetImageViews() const noexcept
{
    return m_image_views;
}

RkBool RenderWindow::IsValid() const noexcept
{
    return m_valid;
}

RkVoid RenderWindow::Begin()
{
    m_current_frame = (m_current_frame + 1) % 2;

    m_device->GetLogicalDevice().waitForFences(1, &m_render_frames[m_current_frame].m_fence, VK_FALSE, UINT64_MAX);

    m_render_frames[m_current_frame].m_command_buffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

    vk::CommandBufferBeginInfo command_buffer_begin_info = {

    };

    m_render_frames[m_current_frame].m_command_buffer.begin(command_buffer_begin_info);

    vk::Viewport viewport = {
        .width    = static_cast<RkFloat>(m_image_extent.width),
        .height   = static_cast<RkFloat>(m_image_extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    vk::Rect2D scissor = {
        .extent = m_image_extent
    };

    m_render_frames[m_current_frame].m_command_buffer.setViewport(0, viewport);
    m_render_frames[m_current_frame].m_command_buffer.setScissor (0, scissor);
}

RkVoid RenderWindow::Render()
{
    m_image_index = m_device->GetLogicalDevice().acquireNextImageKHR(m_swapchain, UINT64_MAX, m_render_frames[m_current_frame].m_image_semaphore).value;

    vk::ClearValue clear_color = { .color = std::array<RkFloat, 4>{0.0f, 0.0f, 0.0f, 1.0f} };

    vk::ClearValue clear_stencil { .depthStencil = {1, 0} };

    std::vector clear_values = {
        clear_color, clear_stencil
    };

    vk::RenderPassBeginInfo render_pass_begin_info = {
        .renderPass = Renderer::render_pass,
        .framebuffer = m_framebuffers[m_image_index],
        .renderArea = {
            .extent = m_image_extent
        },
        .clearValueCount = static_cast<uint32_t>(clear_values.size()),
        .pClearValues = clear_values.data()
    };

    m_render_frames[m_current_frame].m_command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

    m_render_frames[m_current_frame].m_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, Renderer::pipeline);

    g_model->Render(m_render_frames[m_current_frame].m_command_buffer);

    m_render_frames[m_current_frame].m_command_buffer.endRenderPass();
}

RkVoid RenderWindow::End()
{
    m_render_frames[m_current_frame].m_command_buffer.end();

    vk::PipelineStageFlags stage_mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::SubmitInfo submit_info = {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_render_frames[m_current_frame].m_image_semaphore,
        .pWaitDstStageMask = &stage_mask,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_render_frames[m_current_frame].m_command_buffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &m_render_frames[m_current_frame].m_present_semaphore
    };

    m_device->GetLogicalDevice().resetFences(1, &m_render_frames[m_current_frame].m_fence);

    m_device->GetTestQueue().submit(submit_info, m_render_frames[m_current_frame].m_fence);

    vk::PresentInfoKHR present_info = {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_render_frames[m_current_frame].m_present_semaphore,
        .swapchainCount = 1,
        .pSwapchains = &m_swapchain,
        .pImageIndices = &m_image_index
    };

    m_device->GetTestQueue().presentKHR(present_info);
}