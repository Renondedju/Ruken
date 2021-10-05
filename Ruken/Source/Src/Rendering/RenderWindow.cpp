#include "Rendering/RenderWindow.hpp"
#include "Rendering/RenderFrame.hpp"
#include "Rendering/Renderer.hpp"

#include "Windowing/Window.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

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
    auto [result, value] = m_device->GetPhysicalDevice().getSurfaceSupportKHR(m_device->GetGraphicsFamilyIndex(), m_surface);

    if (!value)
        return;
        
    vk::SwapchainCreateInfoKHR swapchain_create_info = {
        .surface          = m_surface,
        .minImageCount    = m_image_count,
        .imageFormat      = m_image_format,
        .imageColorSpace  = m_color_space,
        .imageExtent      = m_image_extent,
        .imageArrayLayers = 1U,
        .imageUsage       = vk::ImageUsageFlagBits::eTransferDst,
        .preTransform     = vk::SurfaceTransformFlagBitsKHR::eIdentity,
        .compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode      = m_present_mode,
        .clipped          = VK_TRUE,
        .oldSwapchain     = in_old_swapchain
    };

    std::tie(result, m_swapchain) = m_device->GetLogicalDevice().createSwapchainKHR(swapchain_create_info);

    if (result == vk::Result::eSuccess)
    {
        m_valid = true;

        std::tie(result, m_images) = m_device->GetLogicalDevice().getSwapchainImagesKHR(m_swapchain);

        /*for (auto const& image : m_images)
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
        }*/
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

RenderWindow::RenderWindow(Logger* in_logger, RenderContext* in_context, RenderDevice* in_device, Window& in_window) noexcept:
    m_logger  {in_logger},
    m_context {in_context},
    m_device  {in_device}
{
    CreateSurface(in_window);
    PickSwapchainImageCount();
    PickSwapchainExtent(in_window.GetFramebufferSize().width, in_window.GetFramebufferSize().height);
    PickSwapchainFormat();
    PickSwapchainPresentMode();
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
}

RenderWindow::~RenderWindow() noexcept
{
    for (RkUint32 i = 0U; i < m_image_count; ++i)
    {
        //m_device->GetLogicalDevice().destroy(m_image_views[i]);
    }

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

RkVoid RenderWindow::Present(RenderFrame& in_frame) noexcept
{
    m_image_index = m_device->GetLogicalDevice().acquireNextImageKHR(m_swapchain, UINT64_MAX, in_frame.GetImageSemaphore()).value;

    auto const& command_buffer = in_frame.RequestCommandBuffer();

    // TODO : Execute final pass.
    vk::CommandBufferBeginInfo command_buffer_begin_info = {

    };

    command_buffer.begin(command_buffer_begin_info);
    {
        vk::ImageMemoryBarrier render_target_barrier = {
            .srcAccessMask    = vk::AccessFlagBits::eColorAttachmentWrite,
            .dstAccessMask    = vk::AccessFlagBits::eTransferRead,
            .oldLayout        = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout        = vk::ImageLayout::eTransferSrcOptimal,
            .image            = in_frame.GetColorTarget().GetImage(),
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            render_target_barrier);

        vk::ImageMemoryBarrier swapchain_barrier = {
            .srcAccessMask    = vk::AccessFlagBits::eNoneKHR,
            .dstAccessMask    = vk::AccessFlagBits::eTransferWrite,
            .oldLayout        = vk::ImageLayout::eUndefined,
            .newLayout        = vk::ImageLayout::eTransferDstOptimal,
            .image            = m_images[m_image_index],
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            swapchain_barrier);

        vk::ImageBlit image_blit = {
            .srcSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            },
            .dstSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .layerCount = 1
            }
        };

        image_blit.srcOffsets[1].x = in_frame.GetColorTarget().GetExtent().width;
        image_blit.srcOffsets[1].y = in_frame.GetColorTarget().GetExtent().height;
        image_blit.srcOffsets[1].z = 1U;
        image_blit.dstOffsets[1].x = m_image_extent.width / 2.0f;
        image_blit.dstOffsets[1].y = m_image_extent.height;
        image_blit.dstOffsets[1].z = 1U;

        command_buffer.blitImage(
            in_frame.GetColorTarget().GetImage(),
            vk::ImageLayout::eTransferSrcOptimal,
            m_images[m_image_index],
            vk::ImageLayout::eTransferDstOptimal,
            1U,
            &image_blit,
            vk::Filter::eLinear);

        render_target_barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
        render_target_barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        render_target_barrier.oldLayout     = vk::ImageLayout::eTransferSrcOptimal;
        render_target_barrier.newLayout     = vk::ImageLayout::eColorAttachmentOptimal;
        
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            render_target_barrier);

        swapchain_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        swapchain_barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
        swapchain_barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
        swapchain_barrier.newLayout     = vk::ImageLayout::ePresentSrcKHR;
        
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            swapchain_barrier);
    }
    command_buffer.end();

    std::vector<vk::SemaphoreSubmitInfoKHR> wait_semaphores_submit_infos = {
        {
            .semaphore = in_frame.GetImageSemaphore()
        },
        {
            .semaphore = in_frame.GetTimelineSemaphore(),
            .value     = in_frame.GetTimelineSemaphoreValue()
        }
    };

    std::vector<vk::CommandBufferSubmitInfoKHR> command_buffer_submit_infos = {
        {
            .commandBuffer = command_buffer
        }
    };

    std::vector<vk::SemaphoreSubmitInfoKHR> signal_semaphores_submit_infos = {
        {
            .semaphore = in_frame.GetPresentSemaphore()
        },
        {
            .semaphore = in_frame.GetTimelineSemaphore(),
            .value     = in_frame.IncrementTimelineSemaphoreValue()
        }
    };

    vk::SubmitInfo2KHR submit_info = {
        .waitSemaphoreInfoCount   = static_cast<RkUint32>(wait_semaphores_submit_infos.size()),
        .pWaitSemaphoreInfos      = wait_semaphores_submit_infos.data(),
        .commandBufferInfoCount   = static_cast<RkUint32>(command_buffer_submit_infos.size()),
        .pCommandBufferInfos      = command_buffer_submit_infos.data(),
        .signalSemaphoreInfoCount = static_cast<RkUint32>(signal_semaphores_submit_infos.size()),
        .pSignalSemaphoreInfos    = signal_semaphores_submit_infos.data()
    };

    m_device->GetGraphicsQueue().Submit(submit_info);

    vk::PresentInfoKHR present_info = {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &in_frame.GetPresentSemaphore(),
        .swapchainCount     = 1,
        .pSwapchains        = &m_swapchain,
        .pImageIndices      = &m_image_index
    };

    m_device->GetGraphicsQueue().Present(present_info);
}