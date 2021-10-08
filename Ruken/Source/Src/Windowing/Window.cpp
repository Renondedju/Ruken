
#include "Rendering/RenderContext.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderFrame.hpp"

#include "Windowing/Window.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Window::Window( Logger* in_logger, RenderContext* in_context, RenderDevice* in_device, WindowParams const& in_params) noexcept:
    m_logger  {in_logger},
    m_context {in_context},
    m_device  {in_device}
{
    CreateWindow(in_params);
    CreateSurface();
    PickSwapchainImageCount();
    PickSwapchainExtent();
    PickSwapchainFormat();
    PickSwapchainPresentMode();
    CreateSwapchain(VK_NULL_HANDLE);
    SetupCallbacks();
}

Window::~Window() noexcept
{
    if (m_swapchain)
        m_device->GetLogicalDevice().destroy(m_swapchain);
    
    if (m_surface)
        m_context->GetInstance().destroy(m_surface);

    if (m_handle)
        glfwDestroyWindow(m_handle);
}

#pragma endregion

#pragma region Static Methods

#pragma region Callbacks

RkVoid Window::WindowPosCallback(GLFWwindow* in_window, RkInt32 const in_x_pos, RkInt32 const in_y_pos) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_moved.Invoke(in_x_pos, in_y_pos);
}

RkVoid Window::WindowSizeCallback(GLFWwindow* in_window, RkInt32 const in_width, RkInt32 const in_height) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_resized.Invoke(in_width, in_height);
}

RkVoid Window::WindowCloseCallback(GLFWwindow* in_window) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_closed.Invoke();
}

RkVoid Window::WindowRefreshCallback(GLFWwindow* in_window) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_refreshed.Invoke();
}

RkVoid Window::WindowFocusCallback(GLFWwindow* in_window, RkInt32 const in_focused) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_focused.Invoke(in_focused);
}

RkVoid Window::WindowIconifyCallback(GLFWwindow* in_window, RkInt32 const in_iconified) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_iconified.Invoke(in_iconified);
}

RkVoid Window::WindowMaximizeCallback(GLFWwindow* in_window, RkInt32 const in_maximized) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_maximized.Invoke(in_maximized);
}

RkVoid Window::FramebufferSizeCallback(GLFWwindow* in_window, RkInt32 const in_width, RkInt32 const in_height) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
    {
        window->PickSwapchainExtent();
        window->CreateSwapchain    (window->m_swapchain);

        window->on_framebuffer_resized.Invoke(in_width, in_height);
    }
}

RkVoid Window::WindowContentScaleCallback(GLFWwindow* in_window, RkFloat const in_x_scale, RkFloat const in_y_scale) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_content_rescaled.Invoke(in_x_scale, in_y_scale);
}

#pragma endregion

Window* Window::GetWindowUserPointer(GLFWwindow* in_window) noexcept
{
    return static_cast<Window*>(glfwGetWindowUserPointer(in_window));
}

#pragma endregion

#pragma region Methods

RkVoid Window::CreateWindow(WindowParams const& in_params) noexcept
{
    glfwDefaultWindowHints();

    // This is needed because GLFW defaults to en OpenGL context otherwise.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Setups the window creation's hints.
    glfwWindowHint(GLFW_RESIZABLE,               in_params.resizable);
    glfwWindowHint(GLFW_VISIBLE,                 in_params.visible);
    glfwWindowHint(GLFW_DECORATED,               in_params.decorated);
    glfwWindowHint(GLFW_FOCUSED,                 in_params.focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY,            in_params.auto_iconified);
    glfwWindowHint(GLFW_FLOATING,                in_params.floating);
    glfwWindowHint(GLFW_MAXIMIZED,               in_params.maximized);
    glfwWindowHint(GLFW_CENTER_CURSOR,           in_params.cursor_centered);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, in_params.transparent_framebuffer);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW,           in_params.focused_on_show);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,        in_params.scale_to_monitor);

    if (in_params.fullscreen)
    {
        GLFWmonitor*       monitor    = glfwGetPrimaryMonitor();
        GLFWvidmode const* video_mode = glfwGetVideoMode     (monitor);

        m_handle = glfwCreateWindow(video_mode->width, video_mode->height, in_params.name, monitor, nullptr);
    }

    else
    {
        m_handle = glfwCreateWindow(
            static_cast<RkInt32>(in_params.size.width),
            static_cast<RkInt32>(in_params.size.height),
            in_params.name,
            nullptr,
            nullptr);
    }

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetWindowOpacity    (m_handle, in_params.opacity);
}

RkVoid Window::CreateSurface() noexcept
{
    VkSurfaceKHR surface;
    
    vk::Result const result = static_cast<vk::Result>(glfwCreateWindowSurface(m_context->GetInstance(), m_handle, nullptr, &surface));

    if (result == vk::Result::eSuccess)
    {
        auto [result, value] = m_device->GetPhysicalDevice().getSurfaceSupportKHR(m_device->GetGraphicsFamilyIndex(), surface);

        if (!value)
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create surface!"))

        m_surface = surface;
        
    }
    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create surface!"))
}

RkVoid Window::PickSwapchainImageCount() noexcept
{
    auto [result, capabilities] =  m_device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);

    m_image_count = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && m_image_count > capabilities.maxImageCount)
        m_image_count = capabilities.maxImageCount;
}

RkVoid Window::PickSwapchainExtent() noexcept
{
    auto [result, capabilities] =  m_device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);

    auto extent = GetFramebufferSize();

    if (capabilities.currentExtent.width != UINT32_MAX)
        m_image_extent = capabilities.currentExtent;
    else
    {
        m_image_extent.width  = std::clamp(extent.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
        m_image_extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
}

RkVoid Window::PickSwapchainFormat() noexcept
{
    auto [result, formats] = m_device->GetPhysicalDevice().getSurfaceFormatsKHR(m_surface);

    m_image_format = formats[0].format;
    m_color_space  = formats[0].colorSpace;
}

RkVoid Window::PickSwapchainPresentMode() noexcept
{
    auto [result, present_modes] =  m_device->GetPhysicalDevice().getSurfacePresentModesKHR(m_surface);

    m_present_mode = present_modes[0];
}

RkVoid Window::CreateSwapchain(vk::SwapchainKHR in_old_swapchain) noexcept
{
    if (m_image_extent.width == 0 || m_image_extent.height == 0)
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

    auto [result, value] = m_device->GetLogicalDevice().createSwapchainKHR(swapchain_create_info);

    if (result == vk::Result::eSuccess)
    {
        m_swapchain = value;

        std::tie(result, m_images) = m_device->GetLogicalDevice().getSwapchainImagesKHR(m_swapchain);
    }

    else
        RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Failed to create swapchain!"))

    if (in_old_swapchain)
        m_device->GetLogicalDevice().destroySwapchainKHR(in_old_swapchain);
}

RkVoid Window::SetupCallbacks() const noexcept
{
    glfwSetWindowPosCallback         (m_handle, &WindowPosCallback);
    glfwSetWindowSizeCallback        (m_handle, &WindowSizeCallback);
    glfwSetWindowCloseCallback       (m_handle, &WindowCloseCallback);
    glfwSetWindowRefreshCallback     (m_handle, &WindowRefreshCallback);
    glfwSetWindowFocusCallback       (m_handle, &WindowFocusCallback);
    glfwSetWindowIconifyCallback     (m_handle, &WindowIconifyCallback);
    glfwSetWindowMaximizeCallback    (m_handle, &WindowMaximizeCallback);
    glfwSetFramebufferSizeCallback   (m_handle, &FramebufferSizeCallback);
    glfwSetWindowContentScaleCallback(m_handle, &WindowContentScaleCallback);
}

RkVoid Window::Present(RenderFrame& in_frame) noexcept
{
    if (m_image_extent.width == 0 || m_image_extent.height == 0)
        return;

    auto image_semaphore   = in_frame.GetSemaphorePool().Request();
    auto present_semaphore = in_frame.GetSemaphorePool().Request();

    m_image_index = m_device->GetLogicalDevice().acquireNextImageKHR(m_swapchain, UINT64_MAX, image_semaphore).value;

    auto command_buffer = in_frame.GetGraphicsCommandPool().Request();

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
        image_blit.dstOffsets[1].x = m_image_extent.width;
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
            .semaphore = image_semaphore
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
            .semaphore = present_semaphore
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
        .pWaitSemaphores    = &present_semaphore,
        .swapchainCount     = 1,
        .pSwapchains        = &m_swapchain,
        .pImageIndices      = &m_image_index
    };

    m_device->GetGraphicsQueue().Present(present_info);

    in_frame.GetSemaphorePool      ().Release(std::move(image_semaphore));
    in_frame.GetSemaphorePool      ().Release(std::move(present_semaphore));
    in_frame.GetGraphicsCommandPool().Release(std::move(command_buffer));
}

#pragma region Setters

RkVoid Window::SetPosition(VkOffset2D const& in_position) const noexcept
{
    glfwSetWindowPos(m_handle, in_position.x, in_position.y);
}

RkVoid Window::SetSizeLimits(VkExtent2D const& in_min_size, VkExtent2D const& in_max_size) const noexcept
{
    glfwSetWindowSizeLimits(m_handle, in_min_size.width, in_min_size.height,
                                      in_max_size.width, in_max_size.height);
}

RkVoid Window::SetAspectRatio(RkInt32 const in_numerator, RkInt32 const in_denominator) const noexcept
{
    glfwSetWindowAspectRatio(m_handle, in_numerator, in_denominator);
}

RkVoid Window::SetSize(VkExtent2D const& in_size) const noexcept
{
    glfwSetWindowSize(m_handle, in_size.width, in_size.height);
}

RkVoid Window::SetOpacity(RkFloat const in_opacity) const noexcept
{
    glfwSetWindowOpacity(m_handle, in_opacity);
}

RkVoid Window::Iconify() const noexcept
{
    glfwIconifyWindow(m_handle);
}

RkVoid Window::Restore() const noexcept
{
    glfwRestoreWindow(m_handle);
}

RkVoid Window::Maximize() const noexcept
{
    glfwMaximizeWindow(m_handle);
}

RkVoid Window::Show() const noexcept
{
    glfwShowWindow(m_handle);
}

RkVoid Window::Hide() const noexcept
{
    glfwHideWindow(m_handle);
}

RkVoid Window::Focus() const noexcept
{
    glfwFocusWindow(m_handle);
}

RkVoid Window::RequestAttention() const noexcept
{
    glfwRequestWindowAttention(m_handle);
}

RkVoid Window::SetFullscreen(RkBool const in_fullscreen) const noexcept
{
    GLFWmonitor*       monitor    = glfwGetPrimaryMonitor();
    GLFWvidmode const* video_mode = glfwGetVideoMode     (monitor);

    if (in_fullscreen)
        glfwSetWindowMonitor(m_handle, monitor, 0, 0, video_mode->width, video_mode->height, GLFW_DONT_CARE);
    else
        glfwSetWindowMonitor(m_handle, nullptr, 0, 0, video_mode->width, video_mode->height, GLFW_DONT_CARE);
}

RkVoid Window::SetDecorated(RkBool const in_decorated) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_DECORATED, in_decorated);
}

RkVoid Window::SetResizable(RkBool const in_resizable) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_RESIZABLE, in_resizable);
}

RkVoid Window::SetFloating(RkBool const in_floating) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_FLOATING, in_floating);
}

RkVoid Window::SetAutoIconified(RkBool const in_auto_iconified) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_AUTO_ICONIFY, in_auto_iconified);
}

RkVoid Window::SetFocusedOnShow(RkBool const in_focused_on_show) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_FOCUS_ON_SHOW, in_focused_on_show);
}

#pragma endregion

#pragma region Getters

RkBool Window::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
}

VkOffset2D Window::GetPosition() const noexcept
{
    VkOffset2D position = {};

    glfwGetWindowPos(m_handle, &position.x, &position.y);

    return position;
}

VkExtent2D Window::GetSize() const noexcept
{
    VkExtent2D size = {};

    glfwGetWindowSize(m_handle, reinterpret_cast<RkInt32*>(&size.width), reinterpret_cast<RkInt32*>(&size.height));

    return size;
}

VkExtent2D Window::GetFramebufferSize() const noexcept
{
    VkExtent2D size = {};

    glfwGetFramebufferSize(m_handle, reinterpret_cast<RkInt32*>(&size.width), reinterpret_cast<RkInt32*>(&size.height));

    return size;
}

VkRect2D Window::GetFrameSize() const noexcept
{
    VkRect2D frame = {};

    glfwGetWindowFrameSize(m_handle, 
                           &frame.offset.x,
                           &frame.offset.y,
                           reinterpret_cast<RkInt32*>(&frame.extent.width),
                           reinterpret_cast<RkInt32*>(&frame.extent.height));

    return frame;
}

VkScale2D Window::GetContentScale() const noexcept
{
    VkScale2D scale = {};

    glfwGetWindowContentScale(m_handle, &scale.x, &scale.y);

    return scale;
}

RkFloat Window::GetOpacity() const noexcept
{
    return glfwGetWindowOpacity(m_handle);
}

RkBool Window::IsFullscreen() const noexcept
{
    return glfwGetWindowMonitor(m_handle) != nullptr;
}

RkBool Window::IsFocused() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUSED);
}

RkBool Window::IsIconified() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_ICONIFIED);
}

RkBool Window::IsMaximized() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_MAXIMIZED);
}

RkBool Window::IsHovered() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_HOVERED);
}

RkBool Window::IsVisible() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_VISIBLE);
}

RkBool Window::IsResizable() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_RESIZABLE);
}

RkBool Window::IsDecorated() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_DECORATED);
}

RkBool Window::IsAutoIconified() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_AUTO_ICONIFY);
}

RkBool Window::IsFloating() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FLOATING);
}

RkBool Window::IsFramebufferTransparent() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_TRANSPARENT_FRAMEBUFFER);
}

RkBool Window::IsFocusedOnShow() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUS_ON_SHOW);
}

#pragma endregion

#pragma endregion